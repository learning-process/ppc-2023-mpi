// Copyright 2023 Simonyan Suren
#include <gtest/gtest.h>
#include <algorithm>
#include <random>
#include "task_3/simonyan_s_quicksort/quicksort.h"

TEST(QUICKSORT, test_comparison_with_stdSort) {
    int ProcRank;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<> dis(0, 10000);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int n = 10;
    double* arr1 = new double[n];
    double* arr2 = new double[n];
    for (int i = 0; i < n; i++) {
        arr1[i] = dis(rng);
        arr2[i] = arr1[i];
    }
    double* arr3 = new double[n];
    arr3 = Sort(arr1, n);
    std::sort(arr2, arr2 + n);
    int ch = 0;
    for (int i = 0; i < n; ++i) {
        if (arr3[i] == arr2[i])
            ch++;
    }
    delete[]arr1;
    delete[]arr2;
    delete[]arr3;
    if (ProcRank == 0)
        ASSERT_EQ(ch, n);
}

TEST(QUICKSORT, test_comparison_with_linery_quicksort) {
    int ProcRank;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<> dis(0, 100);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int n = 100;
    double* arr1 = new double[n];
    double* arr2 = new double[n];
    for (int i = 0; i < n; i++) {
        arr1[i] = dis(rng);
        arr2[i] = arr1[i];
    }
    double* arr3 = new double[n];
    arr3 = Sort(arr1, n);
    int ch = 0;
    quickSortFunction(arr2, 0, n - 1);
    for (int i = 0; i < n; ++i) {
        if (arr3[i] == arr2[i])
            ch++;
    }
    delete[]arr1;
    delete[]arr2;
    delete[]arr3;
    if (ProcRank == 0)
        ASSERT_EQ(ch, n);
}

TEST(QUICKSORT, test_sort_sorted_array) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int n = 10;
    double* arr1 = new double[n];
    double* arr2 = new double[n];
    for (int i = 0; i < n; i++) {
        arr1[i] = i;
        arr2[i] = arr1[i];
    }
    double* arr3 = new double[n];
    arr3 = Sort(arr1, n);
    int ch = 0;
    for (int i = 0; i < n; ++i) {
        if (arr3[i] == arr2[i])
            ch++;
    }
    delete[]arr1;
    delete[]arr2;
    delete[]arr3;
        ASSERT_EQ(ch, n);
}

TEST(QUICKSORT, test_sort_reverce_array) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int n = 10;
    double* arr1 = new double[n];
    double* arr2 = new double[n];
    for (int i = 0; i < n; i++) {
        arr1[i] = n-i-1;
        arr2[i] = i;
    }
    double* arr3 = new double[n];
    arr3 = Sort(arr1, n);
    int ch = 0;
    for (int i = 0; i < n; ++i) {
        if (arr3[i] == arr2[i])
            ch++;
    }
    delete[]arr1;
    delete[]arr2;
    delete[]arr3;
        ASSERT_EQ(ch, n);
}

TEST(QUICKSORT, test_sort_array_of_nulls) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int n = 10;
    double* arr1 = new double[n];
    double* arr2 = new double[n];
    for (int i = 0; i < n; i++) {
        arr1[i] = 0;
        arr2[i] = arr1[i];
    }
    double* arr3 = new double[n];
    arr3 = Sort(arr1, n);
    int ch = 0;
    for (int i = 0; i < n; ++i) {
        if (arr3[i] == arr2[i])
            ch++;
    }
    delete[]arr1;
    delete[]arr2;
    delete[]arr3;
        ASSERT_EQ(ch, n);
}

int main(int argc, char** argv) {
    int result_code = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
