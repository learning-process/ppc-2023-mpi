// Copyright 2023 Salaev Vladislav
#include <gtest/gtest.h>
#include "task_2/salaev_v_lent_gorA_verB/lent_gorA_verB.h"

TEST(Parallel_Operations_MPI, Test_SquareMatrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int row_count = 20, column_count = 20;
    std::vector<int> matrix1, matrix2;

    if (rank == 0) {
        matrix1 = getRandomMatrix(row_count, column_count);
        matrix2 = getRandomMatrix(column_count, row_count);
    }

    std::vector<int> parallel_result = getParallelOperations(
        matrix1, matrix2, row_count, column_count);

    if (rank == 0) {
        std::vector<int> sequential_result = getSequentialOperations(
            matrix1, matrix2, row_count, column_count, row_count);
        ASSERT_EQ(parallel_result, sequential_result);
    }
}

TEST(Parallel_Operations_MPI, Test_SmallMatrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 10, cols = 10;
    std::vector<int> mat1, mat2;

    if (rank == 0) {
        mat1 = getRandomMatrix(rows, cols);
        mat2 = getRandomMatrix(cols, rows);
    }

    auto parallel_result = getParallelOperations(mat1, mat2, rows, cols);

    if (rank == 0) {
        auto sequential_result = getSequentialOperations(
            mat1, mat2, rows, cols, rows);
        ASSERT_EQ(parallel_result, sequential_result);
    }
}

TEST(Parallel_Operations_MPI, Test_NonSquareMatrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 15, cols = 10;
    std::vector<int> mat1, mat2;

    if (rank == 0) {
        mat1 = getRandomMatrix(rows, cols);
        mat2 = getRandomMatrix(cols, rows);
    }

    auto parallel_result = getParallelOperations(mat1, mat2, rows, cols);

    if (rank == 0) {
        auto sequential_result = getSequentialOperations(
            mat1, mat2, rows, cols, rows);
        ASSERT_EQ(parallel_result, sequential_result);
    }
}

TEST(Parallel_Operations_MPI, Test_LargeMatrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 100, cols = 100;
    std::vector<int> mat1, mat2;

    if (rank == 0) {
        mat1 = getRandomMatrix(rows, cols);
        mat2 = getRandomMatrix(cols, rows);
    }

    auto parallel_result = getParallelOperations(mat1, mat2, rows, cols);

    if (rank == 0) {
        auto sequential_result = getSequentialOperations(
            mat1, mat2, rows, cols, rows);
        ASSERT_EQ(parallel_result, sequential_result);
    }
}

TEST(Parallel_Operations_MPI, Test_EmptyMatrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> mat1, mat2;

    auto parallel_result = getParallelOperations(mat1, mat2, 0, 0);

    if (rank == 0) {
        auto sequential_result = getSequentialOperations(mat1, mat2, 0, 0, 0);
        ASSERT_EQ(parallel_result, sequential_result);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    auto& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }

    int result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
