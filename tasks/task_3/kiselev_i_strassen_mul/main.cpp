// Copyright 2023 Kiselev Igor
#include <gtest/gtest.h>
#include "./strassen_mul.h"

TEST(strassen_mul_test, test_2x2_of_1) {
    int rankProc = 0;
    int sizeProc = 0;
    int N = 2;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<std::vector<double>> A(N, std::vector<double>(N));
    std::vector<std::vector<double>> B(N, std::vector<double>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = 1;
            B[i][j] = 1;
        }
    }
    std::vector<std::vector<double>> CSeq(N, std::vector<double>(N));
    CSeq = matrix_multiplication(A, B);

    if (rankProc == 0) {
        std::vector<std::vector<double>> CStr(N, std::vector<double>(N));
        CStr = strassen_mult(A, B);
        ASSERT_EQ(CStr, CSeq);
    }
}

TEST(strassen_mul_test, test_4x4_of_1) {
    int rankProc = 0;
    int sizeProc = 0;
    int N = 4;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<std::vector<double>> A(N, std::vector<double>(N));
    std::vector<std::vector<double>> B(N, std::vector<double>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = 1;
            B[i][j] = 1;
        }
    }
    std::vector<std::vector<double>> CSeq(N, std::vector<double>(N));
    CSeq = matrix_multiplication(A, B);

    if (rankProc == 0) {
        std::vector<std::vector<double>> CStr(N, std::vector<double>(N));
        CStr = strassen_mult(A, B);
        ASSERT_EQ(CStr, CSeq);
    }
}

TEST(strassen_mul_test, test_4x4_of_progression) {
    int rankProc = 0;
    int sizeProc = 0;
    int N = 4;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<std::vector<double>> A(N, std::vector<double>(N));
    std::vector<std::vector<double>> B(N, std::vector<double>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = i * N +j;
            B[i][j] = i * N +j;
        }
    }
    std::vector<std::vector<double>> CSeq(N, std::vector<double>(N));
    CSeq = matrix_multiplication(A, B);

    if (rankProc == 0) {
        std::vector<std::vector<double>> CStr(N, std::vector<double>(N));
        CStr = strassen_mult(A, B);
        ASSERT_EQ(CStr, CSeq);
    }
}

TEST(strassen_mul_test, test_4x4_of_random) {
    int rankProc = 0;
    int sizeProc = 0;
    int N = 4;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<std::vector<double>> A(N, std::vector<double>(N));
    std::vector<std::vector<double>> B(N, std::vector<double>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand_r() % 10;
            B[i][j] = rand_r() % 10;
        }
    }

    std::vector<std::vector<double>> CSeq(N, std::vector<double>(N));
    CSeq = matrix_multiplication(A, B);

    if (rankProc == 0) {
        std::vector<std::vector<double>> CStr(N, std::vector<double>(N));
        CStr = strassen_mult(A, B);
        ASSERT_EQ(CStr, CSeq);
    }
}

TEST(strassen_mul_test, test_10x10_of_random) {
    int rankProc = 0;
    int sizeProc = 0;
    int N = 10;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<std::vector<double>> A(N, std::vector<double>(N));
    std::vector<std::vector<double>> B(N, std::vector<double>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand_r() % 10;
            B[i][j] = rand_r() % 10;
        }
    }

    std::vector<std::vector<double>> CSeq(N, std::vector<double>(N));
    CSeq = matrix_multiplication(A, B);

    if (rankProc == 0) {
        std::vector<std::vector<double>> CStr(N, std::vector<double>(N));
        CStr = strassen_mult(A, B);
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
