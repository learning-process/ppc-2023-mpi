  // Copyright 2023 Mirzakhmedov Alexander

#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "./global_search.h"

const double pi = 3.1415;
const double epsilon = 0.01;

TEST(GlobalSearch, SinMinus1) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    auto fun = [](double x) { return std::sin(x); };
    double left = -pi;
    double right = pi;
    double expectedMin = -1;

    double result = globalSearchParallel(fun, left, right, epsilon * 0.1);

    if (rank == 0) {
        ASSERT_NEAR(expectedMin, result, epsilon);
    }
}

TEST(GlobalSearch, CosMinus1) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    auto fun = [](double x) { return std::cos(x); };
    double left = 0;
    double right = 2 * pi;
    double expectedMin = -1;

    double result = globalSearchParallel(fun, left, right, epsilon * 0.1);

    if (rank == 0) {
        ASSERT_NEAR(expectedMin, result, epsilon);
    }
}

TEST(GlobalSearch, CosZero) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    auto fun = [](double x) { return std::cos(x); };
    double left = -pi / 2;
    double right = pi / 2;
    double expectedMin = 0;

    double result = globalSearchParallel(fun, left, right, epsilon * 0.1);

    if (rank == 0) {
        ASSERT_NEAR(expectedMin, result, epsilon);
    }
}

TEST(GlobalSearch, SquareZero) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    auto fun = [](double x) { return x * x; };
    double left = -100;
    double right = 1000;
    double expectedMin = 0;

    double result = globalSearchParallel(fun, left, right, epsilon * 0.1);

    if (rank == 0) {
        ASSERT_NEAR(expectedMin, result, epsilon);
    }
}

TEST(GlobalSearch, SqrtTwo) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    auto fun = [](double x) { return std::sqrt(x); };
    double left = 4;
    double right = 1000;
    double expectedMin = 2;

    double result = globalSearchParallel(fun, left, right, epsilon * 0.1);

    if (rank == 0) {
        ASSERT_NEAR(expectedMin, result, epsilon);
    }
}

TEST(GlobalSearch, ComplexFun) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    auto fun = [](double x) { return std::sin(2 * x * std::sin(x) * std::cos(x)) + std::sin(-x * std::cos(x)); };
    double left = 2 * pi;
    double right = 3 * pi;
    double expectedMin = -2;

    double result = globalSearchParallel(fun, left, right, epsilon * 0.1);

    if (rank == 0) {
        ASSERT_NEAR(expectedMin, result, epsilon);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    int e_code = RUN_ALL_TESTS();
    MPI_Finalize();
    return e_code;
}
