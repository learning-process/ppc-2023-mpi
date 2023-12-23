// Copyright 2023 Tushentsova Karina
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "task_2/tushentsova_k_odd_even_sort/odd_even_sort.h"

TEST(BubbleSortOddEvenTest, Test_identical_values) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int size_arr = 10000;
    std::vector<int> arr = { 0, 0, 0 }, ref_arr = arr;

    arr = parallBubbleSortOddEven(arr);

    if (rank == 0) {
        ref_arr = bubbleSortOddEven(ref_arr);
        ASSERT_EQ(ref_arr, arr);
    }
}

TEST(BubbleSortOddEvenTest, Test_set_values) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int size_arr = 10000;
    std::vector<int> arr = { 2, 0, 3 }, ref_arr = arr;

    arr = parallBubbleSortOddEven(arr);

    if (rank == 0) {
        ref_arr = bubbleSortOddEven(ref_arr);
        ASSERT_EQ(ref_arr, arr);
    }
}

TEST(BubbleSortOddEvenTest, Test_100_elements) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int size_arr = 100;
    std::vector<int> arr, ref_arr;

    if (rank == 0) {
        arr = getRandomVector(size_arr);
        ref_arr = arr;
    }

    arr = parallBubbleSortOddEven(arr);

    if (rank == 0) {
        ref_arr = bubbleSortOddEven(ref_arr);
        ASSERT_EQ(ref_arr, arr);
    }
}

TEST(BubbleSortOddEvenTest, Test_1000_elements) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int size_arr = 1000;
    std::vector<int> arr, ref_arr;

    if (rank == 0) {
        arr = getRandomVector(size_arr);
        ref_arr = arr;
    }

    arr = parallBubbleSortOddEven(arr);

    if (rank == 0) {
        ref_arr = bubbleSortOddEven(ref_arr);
        ASSERT_EQ(ref_arr, arr);
    }
}

TEST(BubbleSortOddEvenTest, Test_10000_elements) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int size_arr = 10000;
    std::vector<int> arr, ref_arr;

    if (rank == 0) {
        arr = getRandomVector(size_arr);
        ref_arr = arr;
    }

    arr = parallBubbleSortOddEven(arr);

    if (rank == 0) {
        ref_arr = bubbleSortOddEven(ref_arr);
        ASSERT_EQ(ref_arr, arr);
    }
}

int main(int argc, char** argv) {
    int resultCode = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    resultCode = RUN_ALL_TESTS();
    MPI_Finalize();

    return resultCode;
}
