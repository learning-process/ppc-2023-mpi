// Copyright 2023 Nogin Denis

#include <gtest/gtest.h>
#include <iostream>
#include "./algo.h"

TEST(MPI_TESTS, example) {
    int rank = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec = {1, 2, 4, 8, 64, 35, -3, -11, 44, 98, 50};
    std::pair<int, int> parallelResult = parallelMostDifferentNeighborElements(vec);

    if (rank == 0) {
        std::pair<int, int> sequentResult = sequentialMostDifferentNeighborElements(vec);
        ASSERT_EQ(parallelResult, sequentResult);
    }
}

TEST(MPI_TESTS, random) {
    int rank = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sizeVec = 1000000;
    int minValue = -1000000;
    int maxValue = 1000000;

    std::vector<int> vec(randomVector(sizeVec, minValue, maxValue));
    std::pair<int, int> parallelResult = parallelMostDifferentNeighborElements(vec);

    if (rank == 0) {
        std::pair<int, int> sequentResult = sequentialMostDifferentNeighborElements(vec);
        ASSERT_EQ(parallelResult, sequentResult);
    }
}

TEST(MPI_TESTS, two) {
    int rank = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec = {-1, 1};
    std::pair<int, int> parallelResult = parallelMostDifferentNeighborElements(vec);

    if (rank == 0) {
        std::pair<int, int> sequentResult = sequentialMostDifferentNeighborElements(vec);
        ASSERT_EQ(parallelResult, sequentResult);
    }
}

TEST(MPI_TESTS, one) {
    int rank = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec = {1000000};
    std::pair<int, int> parallelResult = parallelMostDifferentNeighborElements(vec);

    if (rank == 0) {
        std::pair<int, int> sequentResult = sequentialMostDifferentNeighborElements(vec);
        ASSERT_EQ(parallelResult, sequentResult);
    }
}

TEST(MPI_TESTS, equal) {
    int rank = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec = {1, 1, 1, 1, 1};
    std::pair<int, int> parallelResult = parallelMostDifferentNeighborElements(vec);

    if (rank == 0) {
        std::pair<int, int> sequentResult = sequentialMostDifferentNeighborElements(vec);
        ASSERT_EQ(parallelResult, sequentResult);
    }
}

TEST(MPI_TESTS, empty) {
    int rank = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec;
    std::pair<int, int> parallelResult = parallelMostDifferentNeighborElements(vec);

    if (rank == 0) {
        std::pair<int, int> sequentResult = sequentialMostDifferentNeighborElements(vec);
        ASSERT_EQ(parallelResult, sequentResult);
    }
}

int main(int argc, char** argv) {
    int result = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    MPI_Init(&argc, &argv);
    int worldRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    if (worldRank != 0)
        delete listeners.Release(listeners.default_result_printer());

    result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}
