// Copyright 2023 Travin Maxim
#include <gtest/gtest.h>
#include "./global_opt.h"

const double pi = 3.1415;
const double epsilon = 0.01;

TEST(GlobalSearch, SinPi) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    auto fun = [](double x) { return std::sin(x); };
    double left = -pi;
    double right = pi;
    double expectedMin = -1;

    double result = globalSearchParallel(fun, left, right, epsilon);

    if (rank == 0) {
        ASSERT_NEAR(expectedMin, result, epsilon);
    }
}

TEST(GlobalSearch, CosPi) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    auto fun = [](double x) { return std::cos(x); };
    double left = 0;
    double right = 2 * pi;
    double expectedMin = -1;

    double result = globalSearchParallel(fun, left, right, epsilon);

    if (rank == 0) {
        ASSERT_NEAR(expectedMin, result, epsilon);
    }
}

TEST(GlobalSearch, CosHalfPi) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    auto fun = [](double x) { return std::cos(x); };
    double left = -pi / 2;
    double right = pi / 2;
    double expectedMin = 0;

    double result = globalSearchParallel(fun, left, right, epsilon);

    if (rank == 0) {
        ASSERT_NEAR(expectedMin, result, epsilon);
    }
}

TEST(GlobalSearch, SinHalfPi) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    auto fun = [](double x) { return std::sin(x); };
    double left = -pi / 2;
    double right = pi / 2;
    double expectedMin = -1;

    double result = globalSearchParallel(fun, left, right, epsilon);

    if (rank == 0) {
        ASSERT_NEAR(expectedMin, result, epsilon);
    }
}

TEST(GlobalSearch, SqrX) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    auto fun = [](double x) { return x * x; };
    double left = -10;
    double right = 100;
    double expectedMin = 0;

    double result = globalSearchParallel(fun, left, right, epsilon);

    if (rank == 0) {
        ASSERT_NEAR(expectedMin, result, epsilon);
    }
}

TEST(GlobalSearch, TripleX) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    auto fun = [](double x) { return x * x * x; };
    double left = -10;
    double right = 10;
    double expectedMin = -1000;

    double result = globalSearchParallel(fun, left, right, epsilon * 0.001);

    if (rank == 0) {
        ASSERT_NEAR(expectedMin, result, epsilon);
    }
}

int main(int argc, char** argv) {
    int result = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
    result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}
