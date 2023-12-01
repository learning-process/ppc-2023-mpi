// Copyright 2023 Vanushkin Dmitry
#include "task_2/vanushkin_d_seidel_method/seidel_method.h"
#include <boost/mpi.hpp>


DoubleVector LocalParallelSeidelMethod(const DoubleMatrix& localA, const DoubleVector& localB,
                                       size_t totalEquationsCount, double eps);

double CalculateError(const DoubleVector& x, const DoubleVector& y);

std::pair<size_t, size_t> CalculateLocalDistributionOffsetAndCount(size_t processCount, size_t processRank, size_t totalEquationsCount);

void DistributeEquationsParams(const DoubleMatrix& a, const DoubleVector& b,
                               DoubleMatrix& localA, DoubleVector& localB);



DoubleVector SequentialSeidelMethod(const DoubleMatrix& a, const DoubleVector& b, double eps) {
    size_t solutionsCount = b.size();

    DoubleVector stepAnswer(solutionsCount, 0);
    DoubleVector nextStepAnswer = stepAnswer;

    do {

        stepAnswer = nextStepAnswer;

        for (size_t x = 0; x < solutionsCount; ++x) {

            double currentAnswer = b[x];

            for (size_t i = 0; i < solutionsCount; i++) {
                if (i != x) {
                    currentAnswer -= a[x][i] * nextStepAnswer[i];
                }
            }

            currentAnswer /= a[x][x];

            nextStepAnswer[x] = currentAnswer;
        }

    } while (norm(stepAnswer, nextStepAnswer) > eps);


    return nextStepAnswer;
}



double norm(const DoubleVector &v1, const DoubleVector &v2) {
    double result = 0;

    for (size_t i = 0; i < v1.size(); i++) {
        result += pow(v2[i] - v1[i], 2);
    }

    return sqrt(result);
}


DoubleVector ParallelSeidelMethod(const DoubleMatrix &a, const DoubleVector &b, double eps) {
    DoubleMatrix localA;
    DoubleVector localB;

    DistributeEquationsParams(a, b, localA, localB);

    return std::move(LocalParallelSeidelMethod(localA, localB, b.size(), eps));
}

void DistributeEquationsParams(const DoubleMatrix& a, const DoubleVector& b,
                               DoubleMatrix& localA, DoubleVector& localB) {

    boost::mpi::communicator world;

    auto [offset, count] = CalculateLocalDistributionOffsetAndCount(
            world.size(), world.rank(), b.size());

    localA.resize(count);
    localB.resize(count);

    for (size_t i = 0; i < count; ++i) {
        localA[i] = a[offset + i];
        localB[i] = b[offset + i];
    }
}

double CalculateError(const std::vector<double> &x, const std::vector<double> &y) {
    double maxError = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        double error = std::abs(x[i] - y[i]);
        if (error > maxError) {
            maxError = error;
        }
    }
    return maxError;
}

std::pair<size_t, size_t> CalculateLocalDistributionOffsetAndCount(size_t processCount, size_t processRank, size_t totalEquationsCount) {

    size_t equationsPerProcess = totalEquationsCount / processCount;

    size_t remainder = totalEquationsCount % processCount;

    size_t offset = processRank * equationsPerProcess + ( processRank < remainder ? processRank : remainder);
    size_t count = equationsPerProcess + (processRank < remainder ? 1 : 0);

    return std::make_pair(offset, count);
}

DoubleVector LocalParallelSeidelMethod(const DoubleMatrix &localA, const DoubleVector &localB, size_t totalEquationsCount, double eps) {

    namespace mpi = boost::mpi;

    mpi::communicator world;
    int rank = world.rank();

    auto [localEquationOffset, localEquationsCount] =
            CalculateLocalDistributionOffsetAndCount(world.size(), rank, totalEquationsCount);

    std::vector<double> x(totalEquationsCount), xNew(totalEquationsCount);

    double localMaxError = eps + 1.0;
    while (localMaxError > eps) {
        for (size_t i = 0; i < localEquationsCount; ++i) {
            double sum = 0.0;
            for (size_t j = 0; j < totalEquationsCount; ++j) {
                if (j != localEquationOffset + i) {
                    sum += localA[i][j] * x[j];
                }
            }
            xNew[localEquationOffset + i] = (localB[i] - sum) / localA[i][localEquationOffset + i];
        }

        // wait while all processes done current iteration
        world.barrier();

        // exchange local solves between processes (actualization)
        for (size_t i = 0; i < world.size(); i++) {
            auto [currentOffset, currentCount] =
                    CalculateLocalDistributionOffsetAndCount(world.size(), i, totalEquationsCount);

            broadcast(world, xNew.data() + currentOffset, currentCount, i);
        }

        localMaxError = CalculateError(x, xNew);

//        mpi::all_reduce(world, localMaxError, mpi::maximum<double>());


        x = xNew;
    }

    return x;

}
