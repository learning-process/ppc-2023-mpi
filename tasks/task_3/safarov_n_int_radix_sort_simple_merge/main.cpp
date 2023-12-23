// Copyright 2023 Safarov Nurlan
#include <gtest/gtest.h>

#include "task_3/safarov_n_int_radix_sort_simple_merge/int_radix_sort_simple_merge.h"

TEST(RadixSortWithSimpleMerge, TestOne) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> randomVector;
    const int sizeOfVector = 60;

    if (rank == 0) {
        randomVector = generateRandomVector(sizeOfVector);
    }

    std::vector<int> resultParallel = radixSortWithSimpleMergeParallel(randomVector, sizeOfVector);

    if (rank == 0) {
        std::vector<int> resultSequential = radixSortWithSimpleMergeSequential(randomVector);
        ASSERT_EQ(resultSequential, resultParallel);
    }
}

TEST(RadixSortWithSimpleMerge, TestTwo) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> randomVector;
    const int sizeOfVector = 10;

    if (rank == 0) {
        randomVector = generateRandomVector(sizeOfVector);
    }

    std::vector<int> resultParallel = radixSortWithSimpleMergeParallel(randomVector, sizeOfVector);

    if (rank == 0) {
        std::vector<int> resultSequential = radixSortWithSimpleMergeSequential(randomVector);
        ASSERT_EQ(resultSequential, resultParallel);
    }
}

TEST(RadixSortWithSimpleMerge, TestThree) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> randomVector;
    const int sizeOfVector = 1;

    if (rank == 0) {
        randomVector = generateRandomVector(sizeOfVector);
    }

    std::vector<int> resultParallel = radixSortWithSimpleMergeParallel(randomVector, sizeOfVector);

    if (rank == 0) {
        std::vector<int> resultSequential = radixSortWithSimpleMergeSequential(randomVector);
        ASSERT_EQ(resultSequential, resultParallel);
    }
}

TEST(RadixSortWithSimpleMerge, TestFour) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> randomVector;
    const int sizeOfVector = 21;

    if (rank == 0) {
        randomVector = generateRandomVector(sizeOfVector);
    }

    std::vector<int> resultParallel = radixSortWithSimpleMergeParallel(randomVector, sizeOfVector);

    if (rank == 0) {
        std::vector<int> resultSequential = radixSortWithSimpleMergeSequential(randomVector);
        ASSERT_EQ(resultSequential, resultParallel);
    }
}

TEST(RadixSortWithSimpleMerge, TestFive) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> randomVector;
    const int sizeOfVector = 5;

    if (rank == 0) {
        randomVector = generateRandomVector(sizeOfVector);
    }

    std::vector<int> resultParallel = radixSortWithSimpleMergeParallel(randomVector, sizeOfVector);

    if (rank == 0) {
        std::vector<int> resultSequential = radixSortWithSimpleMergeSequential(randomVector);
        ASSERT_EQ(resultSequential, resultParallel);
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
    int exec_code = RUN_ALL_TESTS();
    MPI_Finalize();
    return exec_code;
}
