// Copyright 2023 Nogin Denis

#include <gtest/gtest.h>
#include <algorithm>
#include <iostream>
#include "./algo.h"

TEST(MPI_TESTS, example) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec = {2, 1, 8, 4, 64, 35, 3, 11, 44, 98, 50, 66};
    std::vector<int> parallelResult = parallelRadixSort(vec);

    if (rank == 0) {
        std::vector<int> sequentResult = radixSort(vec);
        ASSERT_EQ(parallelResult, sequentResult);
    }
}

TEST(MPI_TESTS, randomvec) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec(randomVector(100, 0, 100));
    std::vector<int> parallelResult = parallelRadixSort(vec);

    if (rank == 0) {
        std::vector<int> sequentResult = radixSort(vec);
        ASSERT_EQ(parallelResult, sequentResult);
    }
}

TEST(MPI_TESTS, empty) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec;
    std::vector<int> parallelResult = parallelRadixSort(vec);

    if (rank == 0) {
        std::vector<int> sequentResult = radixSort(vec);
        ASSERT_EQ(parallelResult, sequentResult);
    }
}

TEST(MPI_TESTS, one) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec = {1};
    std::vector<int> parallelResult = parallelRadixSort(vec);

    if (rank == 0) {
        std::vector<int> sequentResult = radixSort(vec);
        ASSERT_EQ(parallelResult, sequentResult);
    }
}

TEST(MPI_TESTS, equal) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    std::vector<int> parallelResult = parallelRadixSort(vec);

    if (rank == 0) {
        std::vector<int> sequentResult = radixSort(vec);
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
