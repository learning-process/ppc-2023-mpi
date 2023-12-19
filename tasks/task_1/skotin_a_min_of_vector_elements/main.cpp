// Copyright 2023 Skotin Alexander
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "VectorMinFinder.h"

TEST(VectorMinFinderTest, TestWithFixedValues) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    VectorMinFinder minFinder;
    std::vector<int> vector = {15, 6, -3, 12, 9, -5, 10, 7};
    int parallelResult = minFinder.findMinParallel(vector);

    if (rank == 0) {
        int sequentialResult = VectorMinFinder::findMinSequential(vector);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

TEST(VectorMinFinderTest, TestWithOnlyPositiveValues) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    VectorMinFinder minFinder;
    std::vector<int> vector = {10, 20, 30, 40, 50};
    int parallelResult = minFinder.findMinParallel(vector);

    if (rank == 0) {
        int sequentialResult = VectorMinFinder::findMinSequential(vector);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

TEST(VectorMinFinderTest, TestWithOnlyNegativeValues) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    VectorMinFinder minFinder;
    std::vector<int> vector = {-1, -2, -3, -4, -5};
    int parallelResult = minFinder.findMinParallel(vector);

    if (rank == 0) {
        int sequentialResult = VectorMinFinder::findMinSequential(vector);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

TEST(VectorMinFinderTest, TestWithSingleElement) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    VectorMinFinder minFinder;
    std::vector<int> vector = {42};
    int parallelResult = minFinder.findMinParallel(vector);

    if (rank == 0) {
        int sequentialResult = VectorMinFinder::findMinSequential(vector);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

TEST(VectorMinFinderTest, TestWithEmptyVector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    VectorMinFinder minFinder;
    std::vector<int> vector;
    int parallelResult = minFinder.findMinParallel(vector);

    if (rank == 0) {
        int sequentialResult = VectorMinFinder::findMinSequential(vector);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

TEST(VectorMinFinderTest, TestWithRandomValues) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    VectorMinFinder minFinder;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    size_t size = 1000;
    std::vector<int> vector(size);
    std::generate(vector.begin(), vector.end(), []() { return std::rand() % 1000 - 500; });

    int parallelResult = minFinder.findMinParallel(vector);

    if (rank == 0) {
        int sequentialResult = VectorMinFinder::findMinSequential(vector);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }

    int result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}


