// Copyright 2023 Kokin Ivan
#include <gtest/gtest.h>
#include <mpi.h>

#include <iostream>

#include "./lent_horiz_shema.h"

TEST(MPI_TEST, Test1) {
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    int maxSize = 3;
    std::vector<int> A, B, C, secondC;
    A.resize(maxSize * maxSize);
    B.resize(maxSize * maxSize);
    C.resize(maxSize * maxSize);
    secondC.resize(maxSize * maxSize);
    if (rankProc == 0) {
        A = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        B = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    }

    C = schemeMultParallel(&A, &B, &C, maxSize);
    if (rankProc == 0) {
        secondC = schemeMult(&A, &B, &C, maxSize, maxSize);
        ASSERT_EQ(C, secondC);
    }
}

TEST(MPI_TEST, Test2) {
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    int maxSize = 2;
    std::vector<int> A, B, C, secondC;
    A.resize(maxSize * maxSize);
    B.resize(maxSize * maxSize);
    C.resize(maxSize * maxSize);
    secondC.resize(maxSize * maxSize);
    if (rankProc == 0) {
        A = { 40, 20, 10, 80 };
        B = { 75, 36, 65, 91 };
        for (int i = 0; i < maxSize * maxSize; i++) {
            C[i] = 0;
        }
    }

    C = schemeMultParallel(&A, &B, &C, maxSize);
    if (rankProc == 0) {
        secondC = schemeMult(&A, &B, &C, maxSize, maxSize);
        ASSERT_EQ(C, secondC);
    }
}

TEST(MPI_TEST, Test3) {
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    int maxSize = 4;
    std::vector<int> A, B, C, secondC;
    A.resize(maxSize * maxSize);
    B.resize(maxSize * maxSize);
    C.resize(maxSize * maxSize);
    secondC.resize(maxSize * maxSize);
    if (rankProc == 0) {
        A = { 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
        B = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        for (int i = 0; i < maxSize * maxSize; i++) {
            C[i] = 0;
        }
    }

    C = schemeMultParallel(&A, &B, &C, maxSize);
    if (rankProc == 0) {
        secondC = schemeMult(&A, &B, &C, maxSize, maxSize);
        ASSERT_EQ(C, secondC);
    }
}

TEST(MPI_TEST, Test4) {
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    int maxSize = 5;
    std::vector<int> A, B, C, secondC;
    A.resize(maxSize * maxSize);
    B.resize(maxSize * maxSize);
    C.resize(maxSize * maxSize);
    secondC.resize(maxSize * maxSize);
    if (rankProc == 0) {
        A = { 5,  10, 15, 20, 25, 30, 35,  40,  45,  50,  55,  60, 65,
             70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125 };
        B = { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
             0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 };
        for (int i = 0; i < maxSize * maxSize; i++) {
            C[i] = 0;
        }
    }

    C = schemeMultParallel(&A, &B, &C, maxSize);
    if (rankProc == 0) {
        secondC = schemeMult(&A, &B, &C, maxSize, maxSize);
        ASSERT_EQ(C, secondC);
    }
}

TEST(MPI_TEST, Test5) {
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    int maxSize = 3;
    std::vector<int> A, B, C, secondC;
    A.resize(maxSize * maxSize);
    B.resize(maxSize * maxSize);
    C.resize(maxSize * maxSize);
    secondC.resize(maxSize * maxSize);
    if (rankProc == 0) {
        A = { 1, 3, 5, 7, 9, 11, 13, 15, 17 };
        B = { 2, 4, 6, 8, 10, 12, 14, 16, 18 };
        for (int i = 0; i < maxSize * maxSize; i++) {
            C[i] = 0;
        }
    }

    C = schemeMultParallel(&A, &B, &C, maxSize);
    if (rankProc == 0) {
        secondC = schemeMult(&A, &B, &C, maxSize, maxSize);
        ASSERT_EQ(C, secondC);
    }
}

TEST(MPI_TEST, Test6) {
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    int maxSize = 3;
    std::vector<int> A, B, C, secondC;
    A.resize(maxSize * maxSize);
    B.resize(maxSize * maxSize);
    C.resize(maxSize * maxSize);
    secondC.resize(maxSize * maxSize);
    if (rankProc == 0) {
        A = { 2, 4, 6, 8, 10, 12, 14, 16, 18 };
        B = { 1, 3, 5, 7, 9, 11, 13, 15, 17 };
        for (int i = 0; i < maxSize * maxSize; i++) {
            C[i] = 0;
        }
    }

    C = schemeMultParallel(&A, &B, &C, maxSize);
    if (rankProc == 0) {
        secondC = schemeMult(&A, &B, &C, maxSize, maxSize);
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