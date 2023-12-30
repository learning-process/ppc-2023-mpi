// Copyright 2023 Troitskiy Alexandr
#include <gtest/gtest.h>
#include <task_1/troitskiy_a_matrix_col_summa/matrix_col_summa.h>
#include <iostream>
#include <utility>

TEST(ParallelMatrixColsSum, Test1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t rows = 4;
    size_t cols = 6;
    std::vector<double> matrix;

    if (rank == 0) {
        matrix = {
            1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
            1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
            1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
            1.0, 2.0, 3.0, 4.0, 5.0, 6.0
        };
        matrix = transposition(matrix, rows, cols);
    }

    auto tmp = rows;
    rows = cols;
    cols = tmp;

    std::vector<double> parallel_result =
        matrixColsSumParallel(matrix, rows, cols);

    if (rank == 0) {
        std::vector<double> sequential_result =
            matrixColsSumSequential(matrix, rows, cols);
        ASSERT_EQ(sequential_result, parallel_result);
    }
}

TEST(ParallelMatrixColsSum, Test2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t rows = 50;
    size_t cols = 50;
    std::vector<double> matrix;

    if (rank == 0) {
        matrix = std::vector<double>(rows * cols, 1.0);
        matrix = transposition(matrix, rows, cols);
    }

    auto tmp = rows;
    rows = cols;
    cols = tmp;

    std::vector<double> parallel_result =
        matrixColsSumParallel(matrix, rows, cols);

    if (rank == 0) {
        std::vector<double> sequential_result =
            matrixColsSumSequential(matrix, rows, cols);
        ASSERT_EQ(sequential_result, parallel_result);
    }
}

TEST(ParallelMatrixColsSum, Test3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t rows = 10;
    size_t cols = 3;
    std::vector<double> matrix;

    if (rank == 0) {
        matrix = std::vector<double>(rows * cols, 0.0);
    }

    auto tmp = rows;
    rows = cols;
    cols = tmp;

    std::vector<double> parallel_result =
        matrixColsSumParallel(matrix, rows, cols);

    if (rank == 0) {
        std::vector<double> sequential_result =
            matrixColsSumSequential(matrix, rows, cols);
        ASSERT_EQ(sequential_result, parallel_result);
    }
}

TEST(ParallelMatrixColsSum, Test4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t rows = 4;
    size_t cols = 6;
    std::vector<double> matrix;

    if (rank == 0) {
        matrix = {
            1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
            1.0, 2.0, 3.0, 4.0, 5.0, 0.0,
            1.0, 2.0, 3.0, 4.0, 0.0, 0.0,
            1.0, 2.0, 3.0, 0.0, 0.0, 0.0,
            1.0, 2.0, 0.0, 0.0, 0.0, 0.0,
            1.0, 0.0, 0.0, 0.0, 0.0, 0.0
        };
        matrix = transposition(matrix, rows, cols);
    }

    auto tmp = rows;
    rows = cols;
    cols = tmp;

    std::vector<double> parallel_result =
        matrixColsSumParallel(matrix, rows, cols);

    if (rank == 0) {
        std::vector<double> sequential_result =
            matrixColsSumSequential(matrix, rows, cols);
        ASSERT_EQ(sequential_result, parallel_result);
    }
}

TEST(ParallelMatrixColsSum, Test5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t rows = 4;
    size_t cols = 6;
    std::vector<double> matrix;

    if (rank == 0) {
        matrix = {
            0.0, 0.0, 3.0, 4.0, 0.0, 0.0,
            0.0, 2.0, 3.0, 4.0, 5.0, 0.0,
            1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
            1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
            0.0, 2.0, 3.0, 4.0, 5.0, 0.0,
            0.0, 0.0, 3.0, 4.0, 0.0, 0.0
        };
        matrix = transposition(matrix, rows, cols);
    }

    auto tmp = rows;
    rows = cols;
    cols = tmp;

    std::vector<double> parallel_result =
        matrixColsSumParallel(matrix, rows, cols);

    if (rank == 0) {
        std::vector<double> sequential_result =
            matrixColsSumSequential(matrix, rows, cols);
        ASSERT_EQ(sequential_result, parallel_result);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int result = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
