// Copyright 2023 Vanushkin Dmitry
#include <vector>
#include <random>
#include <gtest/gtest.h>
#include <boost/mpi.hpp>
#include "task_2/vanushkin_d_seidel_method/seidel_method.h"

void assertNear(const DoubleVector& a, const DoubleVector& b, double eps) {
    for (size_t i = 0; i < a.size(); i++) {
        ASSERT_NEAR(a[i], b[i], eps);
    }
}

void FillEquationsParametersFollowDiagonalDomination(
        DoubleMatrix& a, DoubleVector& b) { // NOLINT
    std::random_device dev;
    std::mt19937 gen(dev());

    auto equationsCount = b.size();

    for (size_t j = 0; j < equationsCount; ++j) {
        double colSum = 0;
        for (size_t i = 0; i < equationsCount; ++i) {
            a[i * equationsCount + j] = gen() % 1000;
            colSum += a[i * equationsCount + j];
        }
        a[j * equationsCount + j] = std::max(colSum, 1.0);
        b[j] = gen() % 1000;
    }
}

TEST(seidel_method, sequential_test) {
    DoubleMatrix a = {
            5, 0, 0,
            0, 4, 0,
            0, 7, 14,
    };

    DoubleVector b = {10, 2, 28};

    double eps = 0.000001;

    auto result = SequentialSeidelMethod(a, b, eps);
    DoubleVector correctSolution = {2, 0.5, 1.75};

    assertNear(result, correctSolution, eps);
}

TEST(seidel_method, parallel_3x3) {
    boost::mpi::communicator world;

    DoubleMatrix a = {
            5, 0, 0,
            0, 4, 0,
            0, 0, 14
    };

    DoubleVector b = {10, 2, 3.5};

    double eps = 0.001;

    auto parallelResult = ParallelSeidelMethod(a, b, eps);

    if (world.rank() == 0) {
        auto sequentialResult = SequentialSeidelMethod(a, b, eps);
        assertNear(sequentialResult, parallelResult, eps);
    }
}


TEST(seidel_method, parallel_7x7) {
    boost::mpi::communicator world;

    DoubleMatrix a = {
        80, 3, 5, 2, 4, 7, 1,
        9, 45, 1, 4, 8, 3, 2,
        3, 7, 51, 3, 5, 2, 6,
        0, 6, 3, 37, 1, 4, 3,
        1, 8, 2, 11, 35, 1, 4,
        5, 1, 6, 2, 5, 41, 5,
        7, 1, 11, 4, 5, 1, 26
    };

    DoubleVector b = {
        1, 5, 6, 3, 8, 9, 2
    };

    double eps = 0.0001;

    auto parallelResult = ParallelSeidelMethod(a, b, eps);

    if (world.rank() == 0) {
        auto sequentialResult = SequentialSeidelMethod(a, b, eps);
        assertNear(sequentialResult, parallelResult, eps);
    }
}


TEST(seidel_method, parallel_6x6_little_eps) {
    boost::mpi::communicator world;

    DoubleMatrix a = {
        80, 3, 5, 2, 4, 7,
        9, 45, 1, 4, 8, 3,
        3, 7, 51, 3, 5, 2,
        0, 6, 3, 37, 1, 4,
        1, 8, 2, 11, 35, 1,
        5, 1, 6, 2, 5, 41,
        7, 1, 11, 4, 5, 1,
    };

    DoubleVector b = {
        1, 5, 6, 7, 8, 9
    };

    double eps = 0.00000001;

    auto parallelResult = ParallelSeidelMethod(a, b, eps);

    if (world.rank() == 0) {
        auto sequentialResult = SequentialSeidelMethod(a, b, eps);
        assertNear(sequentialResult, parallelResult, eps);
    }
}

TEST(seidel_method, parallel_100x100) {
    boost::mpi::communicator world;

    size_t equationsCount = 100;

    DoubleMatrix a(equationsCount * equationsCount, 0);
    DoubleVector b(equationsCount, 0);

    if (world.rank() == 0) {
        FillEquationsParametersFollowDiagonalDomination(a, b);
    }

    boost::mpi::broadcast(world, a.data(), a.size(), 0);
    boost::mpi::broadcast(world, b.data(), b.size(), 0);


    double eps = 0.001;

    auto parallelResult = ParallelSeidelMethod(a, b, eps);

    if (world.rank() == 0) {
        auto sequentialResult = SequentialSeidelMethod(a, b, eps);
        assertNear(sequentialResult, parallelResult, eps);
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
            ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
