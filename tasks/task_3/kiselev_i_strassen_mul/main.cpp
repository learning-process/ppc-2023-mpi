// Copyright 2023 Kiselev Igor
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include <vector>
#include "./strassen_mul.h"

TEST(strassen_mul_test, test_2x2_of_1) {
    int rankProc = 0;
    int sizeProc = 0;
    int N = 2;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<vector<double>> A(N, vector<double>(N));
    vector<vector<double>> B(N, vector<double>(N));
	for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = 1;
            B[i][j] = 1;
        }
    }
    vector<vector<double>> CSeq(N, vector<double>(N));
    CSeq = matrix_multiplication(A, B);

    if (rankProc == 0) {
        vector<vector<double>> CStr(N, vector<double>(N));
        CStr = strassen_mult(A,B);
        ASSERT_EQ(CStr, CSeq);
    }
}

TEST(strassen_mul_test, test_4x4_of_1) {
    int rankProc = 0;
    int sizeProc = 0;
    int N = 4;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<vector<double>> A(N, vector<double>(N));
    vector<vector<double>> B(N, vector<double>(N));
	for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = 1;
            B[i][j] = 1;
        }
    }
    vector<vector<double>> CSeq(N, vector<double>(N));
    CSeq = matrix_multiplication(A, B);

    if (rankProc == 0) {
        vector<vector<double>> CStr(N, vector<double>(N));
        CStr = strassen_mult(A,B);
        ASSERT_EQ(CStr, CSeq);
    }
}

TEST(strassen_mul_test, test_4x4_of_progression) {
    int rankProc = 0;
    int sizeProc = 0;
    int N = 4;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<vector<double>> A(N, vector<double>(N));
    vector<vector<double>> B(N, vector<double>(N));
	for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = i * N +j;
            B[i][j] = i * N +j;
        }
    }
    vector<vector<double>> CSeq(N, vector<double>(N));
    CSeq = matrix_multiplication(A, B);

    if (rankProc == 0) {
        vector<vector<double>> CStr(N, vector<double>(N));
        CStr = strassen_mult(A,B);
        ASSERT_EQ(CStr, CSeq);
    }
}

TEST(strassen_mul_test, test_4x4_of_random) {
    int rankProc = 0;
    int sizeProc = 0;
    int N = 4;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<vector<double>> A(N, vector<double>(N));
    vector<vector<double>> B(N, vector<double>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }

    vector<vector<double>> CSeq(N, vector<double>(N));
    CSeq = matrix_multiplication(A, B);

    if (rankProc == 0) {
        vector<vector<double>> CStr(N, vector<double>(N));
        CStr = strassen_mult(A,B);
        ASSERT_EQ(CStr, CSeq);
    }
}

TEST(strassen_mul_test, test_10x10_of_random) {
    int rankProc = 0;
    int sizeProc = 0;
    int N = 10;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<vector<double>> A(N, vector<double>(N));
    vector<vector<double>> B(N, vector<double>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }

    vector<vector<double>> CSeq(N, vector<double>(N));
    CSeq = matrix_multiplication(A, B);

    if (rankProc == 0) {
        vector<vector<double>> CStr(N, vector<double>(N));
        CStr = strassen_mult(A,B);
        ASSERT_EQ(CStr, CSeq);
    }
}



int main(int argc, char** argv) {

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    result = RUN_ALL_TESTS();
    MPI_Finalize();
    return 0;
}
