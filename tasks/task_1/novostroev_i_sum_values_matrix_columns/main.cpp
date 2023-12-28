// Copyright 2023 Novostroev Ivan
#include <gtest/gtest.h>
#include <task_1/novostroev_i_sum_values_matrix_columns/sum_val_mat_col.h>
#include <iostream>
#include <utility>

TEST(SumValMatrixCol, Test1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t row = 4;
    size_t col = 6;
    std::vector<double> matrix;

    if (rank == 0) {
        matrix = {
            1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
            1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
            1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
            1.0, 2.0, 3.0, 4.0, 5.0, 6.0
        };
        matrix = transposition(matrix, row, col);
    }

    auto tmp = row;
    row = col;
    col = tmp;

    std::vector<double> parallel_result =
        sumMatrixColParallel(matrix, row, col);

    if (rank == 0) {
        std::vector<double> sequential_result =
            sumMatrixColEquential(matrix, row, col);
        ASSERT_EQ(sequential_result, parallel_result);
    }
}

TEST(SumValMatrixCol, Test2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t row = 50;
    size_t col = 50;
    std::vector<double> matrix;

    if (rank == 0) {
        matrix = std::vector<double>(row * col, 1.0);
        matrix = transposition(matrix, row, col);
    }

    auto tmp = row;
    row = col;
    col = tmp;

    std::vector<double> parallel_result =
        sumMatrixColParallel(matrix, row, col);

    if (rank == 0) {
        std::vector<double> sequential_result =
            sumMatrixColEquential(matrix, row, col);
        ASSERT_EQ(sequential_result, parallel_result);
    }
}

TEST(SumValMatrixCol, Test3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t row = 10;
    size_t col = 3;
    std::vector<double> matrix;

    if (rank == 0) {
        matrix = std::vector<double>(row * col, 0.0);
    }

    auto tmp = row;
    row = col;
    col = tmp;

    std::vector<double> parallel_result =
        sumMatrixColParallel(matrix, row, col);

    if (rank == 0) {
        std::vector<double> sequential_result =
            sumMatrixColEquential(matrix, row, col);
        ASSERT_EQ(sequential_result, parallel_result);
    }
}

TEST(SumValMatrixCol, Test4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t row = 4;
    size_t col = 6;
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
        matrix = transposition(matrix, row, col);
    }

    auto tmp = row;
    row = col;
    col = tmp;

    std::vector<double> parallel_result =
        sumMatrixColParallel(matrix, row, col);

    if (rank == 0) {
        std::vector<double> sequential_result =
            sumMatrixColEquential(matrix, row, col);
        ASSERT_EQ(sequential_result, parallel_result);
    }
}

TEST(SumValMatrixCol, Test5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t row = 4;
    size_t col = 6;
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
        matrix = transposition(matrix, row, col);
    }

    auto tmp = row;
    row = col;
    col = tmp;

    std::vector<double> parallel_result =
        sumMatrixColParallel(matrix, row, col);

    if (rank == 0) {
        std::vector<double> sequential_result =
            sumMatrixColEquential(matrix, row, col);
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
