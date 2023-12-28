// Copyright 2023 Kiselev Igor
#include <gtest/gtest.h>
#include "task_2/kiselev_i_mul_tape_matrix/mul_tape_matrix.h"

TEST(MulMatrix_test, test_3x3_of_1) {
    int rankProc = 0;
    int sizeProc = 0;
    int n = 3;
    int m = 3;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    std::vector<int> vecA(n * m);
    std::vector<int> vecB(m * n);
    if (rankProc == 0) {
        for (int i = 0; i < n * m; i++) {
            vecA[i] = 1;
            vecB[i] = 1;
        }
    }
    std::vector<int> resPar = ParMulMatrix(&vecA, &vecB, n, m);
    if (rankProc == 0) {
        std::vector<int> resSeq = SeqMulMatrix(vecA, vecB, n, m, n);
        ASSERT_EQ(resPar, resSeq);
    }
}

TEST(MulMatrix_test, test_3x3_of_progression) {
    int rankProc = 0;
    int sizeProc = 0;
    int n = 3;
    int m = 3;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    std::vector<int> vecA(n * m);
    std::vector<int> vecB(m * n);
    for (int i = 0; i < n * m; i++) {
        vecA[i] = i;
        vecB[i] = i;
    }

    std::vector<int> resPar = ParMulMatrix(&vecA, &vecB, n, m);

    if (rankProc == 0) {
        std::vector<int> resSeq = SeqMulMatrix(vecA, vecB, n, m, n);
        ASSERT_EQ(resPar, resSeq);
    }
}

TEST(MulMatrix_test, test_4x4_and_4x4_of_1) {
    int rankProc = 0;
    int sizeProc = 0;
    int n = 4;
    int m = 4;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);

    std::vector<int> vecA(n * m);
    std::vector<int> vecB(m * n);
    for (int i = 0; i < n * m; i++) {
        vecA[i] = 1;
        vecB[i] = 1;
    }
    std::vector<int> resPar = ParMulMatrix(&vecA, &vecB, n, m);

    if (rankProc == 0) {
        std::vector<int> resSeq = SeqMulMatrix(vecA, vecB, n, m, n);
        ASSERT_EQ(resPar, resSeq);
    }
}

TEST(MulMatrix_test, test_5x5_and_5x5_of_progression) {
    int rankProc = 0;
    int sizeProc = 0;
    int n = 5;
    int m = 5;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    std::vector<int> vecA(n * m);
    std::vector<int> vecB(n * m);
    for (int i = 0; i < n * m; i++) {
        vecA[i] = i;
        vecB[i] = i;
    }

    std::vector<int> resPar = ParMulMatrix(&vecA, &vecB, n, m);

    if (rankProc == 0) {
        std::vector<int> resSeq = SeqMulMatrix(vecA, vecB, n, m, n);
        ASSERT_EQ(resPar, resSeq);
    }
}

TEST(MulMatrix_test, test_6x6_and_6x6_of_progression) {
    int rankProc = 0;
    int sizeProc = 0;
    int n = 6;
    int m = 6;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    std::vector<int> vecA(n * m);
    std::vector<int> vecB(n * m);
    for (int i = 0; i < n * m; i++) {
        vecA[i] = i;
        vecB[i] = i;
    }
    std::vector<int> resPar = ParMulMatrix(&vecA, &vecB, n, m);

    if (rankProc == 0) {
        std::vector<int> resSeq = SeqMulMatrix(vecA, vecB, n, m, n);
        ASSERT_EQ(resPar, resSeq);
    }
}

int main(int argc, char** argv) {
    int result = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);

    int rankProc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

    if (rankProc != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }

    result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}
