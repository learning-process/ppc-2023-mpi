// Copyright 2023 Troitskiy Alexandr

#include <gtest/gtest.h>
#include <mpi.h>
#include "./double_radix_sort.h"


void print_array(std::vector<double> a, int n) {
    std::cout << "================\n";
    for (int i = 0; i < n; i++) {
        std::cout << a[i] << ' ';
    }
    std::cout << "\n";
    std::cout << "================\n";
}

TEST(MPI_TESTS, Test1) {
    int rank = 0;
    int countProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    int n = 16;
    std::vector<double>a, b;
    if (rank == 0) {
        a = getRandomInput(n, 1, 100);
        b = a;
        sort(b.begin(), b.end());
    }
    a = ParallelRadixSortDouble(a, n);
    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            ASSERT_EQ(a[i], b[i]);
        }
    }
}

TEST(MPI_TESTS, Test2) {
    int rank = 0;
    int countProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    int n = 45;
    std::vector<double>a, b;
    if (rank == 0) {
        a = getRandomInput(n, 1, 100);
        b = a;
        sort(b.begin(), b.end());
    }
    a = ParallelRadixSortDouble(a, n);
    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            ASSERT_EQ(a[i], b[i]);
        }
    }
}


TEST(MPI_TESTS, Test_all_neg) {
    int rank = 0;
    int countProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    int n = 58;
    std::vector<double>a, b;
    if (rank == 0) {
        a = getRandomInput(n, 1, 100);
        for (int i = 0; i < n; i++) {
            a[i] *= -1;
        }
        b = a;
        sort(b.begin(), b.end());
    }
    a = ParallelRadixSortDouble(a, n);
    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            ASSERT_EQ(a[i], b[i]);
        }
    }
}

TEST(MPI_TESTS, Test_small_size) {
    int rank = 0;
    int countProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    int n = 3;
    std::vector<double>a, b;
    if (rank == 0) {
        a = getRandomInput(n, 1, 100);
        b = a;
        sort(b.begin(), b.end());
    }
    a = ParallelRadixSortDouble(a, n);
    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            ASSERT_EQ(a[i], b[i]);
        }
    }
}

TEST(MPI_TESTS, Test_big_size) {
    int rank = 0;
    int countProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    int n = 450;
    std::vector<double>a, b;
    if (rank == 0) {
        a = getRandomInput(n, 1, 100);
        b = a;
        sort(b.begin(), b.end());
    }
    a = ParallelRadixSortDouble(a, n);
    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            ASSERT_EQ(a[i], b[i]);
        }
    }
}

int main(int argc, char** argv) {
    int result = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners =
            ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    if (rankProc != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
