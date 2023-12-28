// Copyright 2023 Kochetov Nikolay
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "task_2/kochetov_n_multiplicate_matrix/multiplicate_matrix.h"



TEST(MultiplicateMatrix, Test1) {
    int rank_proc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_proc);
    int max_size = 3;
    std::vector<int> A, B, C, secondC;
    A.resize(max_size * max_size);
    B.resize(max_size * max_size);
    C.resize(max_size * max_size);
    secondC.resize(max_size * max_size);
    if (rank_proc == 0) {
        A = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        B = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        for (int i = 0; i < max_size * max_size; i++) {
            C[i] = 0;
        }
    }

    C = MultiplicateMatrixP(&A, &B, &C, max_size);
    if (rank_proc == 0) {
        secondC = MultiplicateMatrix(&A, &B, &C, max_size, max_size);
        ASSERT_EQ(C, secondC);
    }
}

TEST(MultiplicateMatrix, Test2) {
    int rank_proc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_proc);
    int max_size = 2;
    std::vector<int> A, B, C, secondC;
    A.resize(max_size * max_size);
    B.resize(max_size * max_size);
    C.resize(max_size * max_size);
    secondC.resize(max_size * max_size);
    if (rank_proc == 0) {
        A = {40, 20, 10, 80};
        B = {75, 36, 65, 91};
        for (int i = 0; i < max_size * max_size; i++) {
            C[i] = 0;
        }
    }

    C = MultiplicateMatrixP(&A, &B, &C, max_size);
    if (rank_proc == 0) {
        secondC = MultiplicateMatrix(&A, &B, &C, max_size, max_size);
        ASSERT_EQ(C, secondC);
    }
}

TEST(MultiplicateMatrix, Test3) {
    int rank_proc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_proc);
    int max_size = 4;
    std::vector<int> A, B, C, secondC;
    A.resize(max_size * max_size);
    B.resize(max_size * max_size);
    C.resize(max_size * max_size);
    secondC.resize(max_size * max_size);
    if (rank_proc == 0) {
        A = {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
        B = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        for (int i = 0; i < max_size * max_size; i++) {
            C[i] = 0;
        }
    }

    C = MultiplicateMatrixP(&A, &B, &C, max_size);
    if (rank_proc == 0) {
        secondC = MultiplicateMatrix(&A, &B, &C, max_size, max_size);
        ASSERT_EQ(C, secondC);
    }
}

TEST(MultiplicateMatrix, Test4) {
    int rank_proc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_proc);
    int max_size = 5;
    std::vector<int> A, B, C, secondC;
    A.resize(max_size * max_size);
    B.resize(max_size * max_size);
    C.resize(max_size * max_size);
    secondC.resize(max_size * max_size);
    if (rank_proc == 0) {
        A = {5,  10, 15, 20, 25, 30, 35,  40,  45,  50,  55,  60, 65,
            70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125};
        B = {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
            0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1};
        for (int i = 0; i < max_size * max_size; i++) {
            C[i] = 0;
        }
    }

    C = MultiplicateMatrixP(&A, &B, &C, max_size);
    if (rank_proc == 0) {
        secondC = MultiplicateMatrix(&A, &B, &C, max_size, max_size);
        ASSERT_EQ(C, secondC);
    }
}

TEST(MultiplicateMatrix, Test5) {
    int rank_proc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_proc);
    int max_size = 3;
    std::vector<int> A, B, C, secondC;
    A.resize(max_size * max_size);
    B.resize(max_size * max_size);
    C.resize(max_size * max_size);
    secondC.resize(max_size * max_size);
    if (rank_proc == 0) {
        A = {1, 3, 5, 7, 9, 11, 13, 15, 17};
        B = {2, 4, 6, 8, 10, 12, 14, 16, 18};
        for (int i = 0; i < max_size * max_size; i++) {
            C[i] = 0;
        }
    }

    C = MultiplicateMatrixP(&A, &B, &C, max_size);
    if (rank_proc == 0) {
        secondC = MultiplicateMatrix(&A, &B, &C, max_size, max_size);
        ASSERT_EQ(C, secondC);
    }
}

TEST(MultiplicateMatrix, Test6) {
    int rank_proc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_proc);
    int max_size = 3;
    std::vector<int> A, B, C, secondC;
    A.resize(max_size * max_size);
    B.resize(max_size * max_size);
    C.resize(max_size * max_size);
    secondC.resize(max_size * max_size);
    if (rank_proc == 0) {
        A = {2, 4, 6, 8, 10, 12, 14, 16, 18};
        B = {1, 3, 5, 7, 9, 11, 13, 15, 17};
        for (int i = 0; i < max_size * max_size; i++) {
            C[i] = 0;
        }
    }

    C = MultiplicateMatrixP(&A, &B, &C, max_size);
    if (rank_proc == 0) {
        secondC = MultiplicateMatrix(&A, &B, &C, max_size, max_size);
        ASSERT_EQ(C, secondC);
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int result_code = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    if (proc_rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
