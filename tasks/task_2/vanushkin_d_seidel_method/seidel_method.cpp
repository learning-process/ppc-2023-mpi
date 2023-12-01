// Copyright 2023 Vanushkin Dmitry
#include "task_2/vanushkin_d_seidel_method/seidel_method.h"
#include <boost/mpi.hpp>
#include <utility>

DoubleVector LocalParallelSeidelMethod(
        const DoubleMatrix& localA, const DoubleVector& localB,
        size_t totalEquationsCount, double eps);

double CalculateError(const DoubleVector& v1, const DoubleVector& v2);

std::pair<size_t, size_t> CalculateLocalDistributionOffsetAndCount(
        size_t processCount, size_t processRank,
        size_t totalEquationsCount);

void DistributeEquationsParams(
        const DoubleMatrix& a, const DoubleVector& b,
        DoubleMatrix& localA, DoubleVector& localB); // NOLINT

DoubleVector SequentialSeidelMethod(
        const DoubleMatrix& a, const DoubleVector& b, double eps
) {
    size_t solutionsCount = b.size();

    DoubleVector stepAnswer(solutionsCount, 0);
    DoubleVector nextStepAnswer = stepAnswer;

    do {
        stepAnswer = nextStepAnswer;

        for (size_t x = 0; x < solutionsCount; ++x) {
            double currentAnswer = b[x];

            for (size_t i = 0; i < solutionsCount; i++) {
                if (i != x) {
                    currentAnswer -= a[x * solutionsCount + i] * stepAnswer[i];
                }
            }

            currentAnswer /= a[x * solutionsCount + x];

            nextStepAnswer[x] = currentAnswer;
        }
    } while (CalculateError(stepAnswer, nextStepAnswer) > eps);

    return nextStepAnswer;
}



double CalculateError(const DoubleVector& v1, const DoubleVector& v2) {
    double result = 0;

    for (size_t i = 0; i < v1.size(); i++) {
        result += pow(v2[i] - v1[i], 2);
    }

    return sqrt(result);
}


DoubleVector ParallelSeidelMethod(
        const DoubleMatrix &a, const DoubleVector &b, double eps
) {
    DoubleMatrix localA;
    DoubleVector localB;

    DistributeEquationsParams(a, b, localA, localB);

    return std::move(LocalParallelSeidelMethod(localA, localB, b.size(), eps));
}

void DistributeEquationsParams(
        const DoubleMatrix& a, const DoubleVector& b,
        DoubleMatrix& localA, DoubleVector& localB // NOLINT
) {
    boost::mpi::communicator world;

    auto equationsCount = b.size();

    auto [offset, count] = CalculateLocalDistributionOffsetAndCount(
            world.size(), world.rank(), b.size());

    localA.resize(count * equationsCount);
    localB.resize(count);

    for (size_t i = 0; i < count; ++i) {
        localB[i] = b[offset + i];
        for (size_t j = 0; j < equationsCount; j++)
            localA[i * equationsCount + j] =
                    a[(offset + i) * equationsCount + j];
    }
}

std::pair<size_t, size_t> CalculateLocalDistributionOffsetAndCount(
        size_t processCount, size_t processRank, size_t totalEquationsCount
) {
    size_t equationsPerProcess = totalEquationsCount / processCount;

    size_t remainder = totalEquationsCount % processCount;

    size_t offset = processRank * equationsPerProcess +
            (processRank < remainder ? processRank : remainder);
    size_t count = equationsPerProcess + (processRank < remainder ? 1 : 0);

    return std::make_pair(offset, count);
}

DoubleVector LocalParallelSeidelMethod(
        const DoubleMatrix &localA, const DoubleVector &localB,
        size_t totalEquationsCount, double eps
) {
    namespace mpi = boost::mpi;

    mpi::communicator world;
    int rank = world.rank();

    auto [localEquationOffset, localEquationsCount] =
            CalculateLocalDistributionOffsetAndCount(
                    world.size(), rank,
                    totalEquationsCount);

    std::vector<double> x(totalEquationsCount), xNew(totalEquationsCount);

    do {
        x = xNew;
        for (size_t i = 0; i < localEquationsCount; ++i) {
            double sum = 0.0;
            for (size_t j = 0; j < totalEquationsCount; ++j) {
                if (j != localEquationOffset + i) {
                    sum += localA[i * totalEquationsCount + j] * x[j];
                }
            }
            xNew[localEquationOffset + i] = (localB[i] - sum) /
                    localA[i * totalEquationsCount + localEquationOffset + i];
        }

        // exchange local solves between processes (actualization)
        for (size_t i = 0; i < world.size(); i++) {
            auto [currentOffset, currentCount] =
                CalculateLocalDistributionOffsetAndCount(
                    world.size(),i, // NOLINT
                    totalEquationsCount);

            broadcast(world, xNew.data() + currentOffset, currentCount, i);
        }
    } while (CalculateError(x, xNew) > eps);

    return x;
}
