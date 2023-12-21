// Copyright 2023 Smirnov Leonid
#include <task_3/smirnov_l_search_area/search_area.h>

#include <gtest/gtest.h>
#include <vector>
#include <iostream>

double linearFunction(double* x) {
    return *x;
}

double quadraticFunction(double* x) {
    return (*x) * (*x);
}

double exponentialFunction(double* x) {
    return exp(*x);
}

double cubicFunction(double* x) {
    return (*x) * (*x) * (*x);
}

double cosFunction(double* x) {
    return cos(*x);
}

TEST(GlobalOptimizationArea, LinearFunction) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double lowerBound = -5.0;
    double upperBound = 5.0;
    double accuracy = 0.001;
    int numberPartitions = 1000;

    double parallelResult = ApplyGlobalOptimizationParrallel(
        linearFunction,
        lowerBound,
        upperBound,
        numberPartitions,
        accuracy);

    if (rank == 0) {
        double sequentialResult = ApplyGlobalOptimizationSequential(
            linearFunction,
            lowerBound,
            upperBound,
            numberPartitions,
            accuracy);
        ASSERT_NEAR(parallelResult, sequentialResult, accuracy);
    }
}

TEST(GlobalOptimizationArea, QuadraticFunction) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double lowerBound = 1.5;
    double upperBound = 400;
    double accuracy = 0.0001;
    int numberPartitions = 1000;

    double parallelResult = ApplyGlobalOptimizationParrallel(
        quadraticFunction,
        lowerBound,
        upperBound,
        numberPartitions,
        accuracy);

    if (rank == 0) {
        double sequentialResult = ApplyGlobalOptimizationSequential(
            quadraticFunction,
            lowerBound,
            upperBound,
            numberPartitions,
            accuracy);
        ASSERT_NEAR(parallelResult, sequentialResult, accuracy);
    }
}

TEST(GlobalOptimizationArea, ExponentialFunction) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double lowerBound = -2.0;
    double upperBound = 2.0;
    double accuracy = 0.001;
    int numberPartitions = 1000;

    double parallelResult = ApplyGlobalOptimizationParrallel(
        exponentialFunction,
        lowerBound,
        upperBound,
        numberPartitions,
        accuracy);

    if (rank == 0) {
        double sequentialResult = ApplyGlobalOptimizationSequential(
            exponentialFunction,
            lowerBound,
            upperBound,
            numberPartitions,
            accuracy);
        ASSERT_NEAR(parallelResult, sequentialResult, accuracy);
    }
}

TEST(GlobalOptimizationArea, CubicFunction) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double lowerBound = -2.0;
    double upperBound = 2.0;
    double accuracy = 0.001;
    int numberPartitions = 1000;

    double parallelResult = ApplyGlobalOptimizationParrallel(
        cosFunction,
        lowerBound,
        upperBound,
        numberPartitions,
        accuracy);

    if (rank == 0) {
        double sequentialResult = ApplyGlobalOptimizationSequential(
            cosFunction,
            lowerBound,
            upperBound,
            numberPartitions,
            accuracy);
        ASSERT_NEAR(parallelResult, sequentialResult, accuracy);
    }
}

TEST(GlobalOptimizationArea, CosFunction) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double lowerBound = -2.0;
    double upperBound = 2.0;
    double accuracy = 0.001;
    int numberPartitions = 1000;

    double parallelResult = ApplyGlobalOptimizationParrallel(
        cosFunction,
        lowerBound,
        upperBound,
        numberPartitions,
        accuracy);

    if (rank == 0) {
        double sequentialResult = ApplyGlobalOptimizationSequential(
            cosFunction,
            lowerBound,
            upperBound,
            numberPartitions,
            accuracy);
        ASSERT_NEAR(parallelResult, sequentialResult, accuracy);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int result = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}
