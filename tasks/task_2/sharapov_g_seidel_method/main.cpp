// Copyright 2023 Sharapov Georgiy
#include <mpi.h>
#include <gtest/gtest.h>
#include <cmath>
#include "task_2/sharapov_g_seidel_method/seidel_method.h"

TEST(Seidel_Method_MPI, Test_5) {
    int size = 5;

    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    double* matrix = nullptr;
    if (ProcRank == 0) matrix = generateMatrix(size);
    double eps = 0.00001;

    double* ans = seidelMethod(matrix, size, eps);

    if (ProcRank == 0) {
        bool correct = true;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                matrix[i * (size + 1) + size] -= matrix[i * (size + 1) + j] * ans[j];
            }
            if (fabs(matrix[i * (size + 1) + size]) > eps) {
                correct = false;
                break;
            }
        }
        ASSERT_TRUE(correct);
    }
}

TEST(Seidel_Method_MPI, Test_25) {
    int size = 25;

    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    double* matrix = nullptr;
    if (ProcRank == 0) matrix = generateMatrix(size);
    double eps = 0.00001;

    double* ans = seidelMethod(matrix, size, eps);

    if (ProcRank == 0) {
        bool correct = true;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                matrix[i * (size + 1) + size] -= matrix[i * (size + 1) + j] * ans[j];
            }
            if (fabs(matrix[i * (size + 1) + size]) > eps) {
                correct = false;
                break;
            }
        }
        ASSERT_TRUE(correct);
    }
}

TEST(Seidel_Method_MPI, Test_50) {
    int size = 50;

    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    double* matrix = nullptr;
    if (ProcRank == 0) matrix = generateMatrix(size);
    double eps = 0.00001;

    double* ans = seidelMethod(matrix, size, eps);

    if (ProcRank == 0) {
        bool correct = true;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                matrix[i * (size + 1) + size] -= matrix[i * (size + 1) + j] * ans[j];
            }
            if (fabs(matrix[i * (size + 1) + size]) > eps) {
                correct = false;
                break;
            }
        }
        ASSERT_TRUE(correct);
    }
}

TEST(Seidel_Method_MPI, Test_75) {
    int size = 75;

    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    double* matrix = nullptr;
    if (ProcRank == 0) matrix = generateMatrix(size);
    double eps = 0.00001;

    double* ans = seidelMethod(matrix, size, eps);

    if (ProcRank == 0) {
        bool correct = true;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                matrix[i * (size + 1) + size] -= matrix[i * (size + 1) + j] * ans[j];
            }
            if (fabs(matrix[i * (size + 1) + size]) > eps) {
                correct = false;
                break;
            }
        }
        ASSERT_TRUE(correct);
    }
}

TEST(Seidel_Method_MPI, Test_100) {
    int size = 100;

    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    double* matrix = nullptr;
    if (ProcRank == 0) matrix = generateMatrix(size);
    double eps = 0.00001;

    double* ans = seidelMethod(matrix, size, eps);

    if (ProcRank == 0) {
        bool correct = true;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                matrix[i * (size + 1) + size] -= matrix[i * (size + 1) + j] * ans[j];
            }
            if (fabs(matrix[i * (size + 1) + size]) > eps) {
                correct = false;
                break;
            }
        }
        ASSERT_TRUE(correct);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (ProcRank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }

    int result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}

