/* Copyright 2023 Belan Vadim */

#include <mpi.h>
#include <gtest/gtest.h>
#include <iostream>
#include "task_1/belan_v_sum_elem_matrix/matrix_mpi.h"

// Тест для проверки функции calcMatrixSum
TEST(MatrixSumTest, SumCorrect) {
    int size = 4;
    int** matrix = new int*[size];
    for (int i = 0; i < size; ++i) {
        matrix[i] = new int[size];
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = i * size + j + 1;
        }
    }

    // Вычисление ожидаемой суммы
    int expected_sum = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            expected_sum += matrix[i][j];
        }
    }

    // Вычисление суммы с использованием функции
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int sum = calcMatrixSum(matrix, size, world_rank, world_size);

    // Проверка соответствия сумм
    ASSERT_EQ(sum, expected_sum);

    deleteMatrix(matrix, size);
}

// Второй тест для проверки функции calcMatrixSum
TEST(MatrixSumTest, EmptyMatrix) {
    int size = 0;
    int** matrix = nullptr;

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int sum = calcMatrixSum(matrix, size, world_rank, world_size);

    // Пустая матрица должна давать сумму 0 для каждого процесса
    ASSERT_EQ(sum, 0);
}

// Третий тест для проверки функции calcMatrixSum
TEST(MatrixSumTest, NegativeValues) {
    int size = 4;
    int** matrix = new int*[size];
    for (int i = 0; i < size; ++i) {
        matrix[i] = new int[size];
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = -(i * size + j + 1);
        }
    }

    // Вычисление ожидаемой суммы
    int expected_sum = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            expected_sum += matrix[i][j];
        }
    }

    // Вычисление суммы с использованием функции
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int sum = calcMatrixSum(matrix, size, world_rank, world_size);

    // Проверка соответствия сумм
    ASSERT_EQ(sum, expected_sum);

    deleteMatrix(matrix, size);
}

// Четвертый тест для проверки функции calcMatrixSum
TEST(MatrixSumTest, LargeMatrix) {
    int size = 1000;
    int** matrix = new int*[size];
    for (int i = 0; i < size; ++i) {
        matrix[i] = new int[size];
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = i * size + j + 1;
        }
    }

    // Вычисление ожидаемой суммы
    int expected_sum = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            expected_sum += matrix[i][j];
        }
    }

    // Вычисление суммы с использованием функции
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int sum = calcMatrixSum(matrix, size, world_rank, world_size);

    // Проверка соответствия сумм
    ASSERT_EQ(sum, expected_sum);

    deleteMatrix(matrix, size);
}

// Пятый тест для проверки функции calcMatrixSum
TEST(MatrixSumTest, RowWithNegativeValues) {
    int size = 4;
    int** matrix = new int*[size];
    for (int i = 0; i < size; ++i) {
        matrix[i] = new int[size];
        for (int j = 0; j < size; ++j) {
            if (i == 2) {
                matrix[i][j] = -(i * size + j + 1);
            } else {
                matrix[i][j] = i * size + j + 1;
            }
        }
    }

    // Вычисление ожидаемой суммы
        int expected_sum = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            expected_sum += matrix[i][j];
        }
    }

    // Вычисление суммы с использованием функции
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int sum = calcMatrixSum(matrix, size, world_rank, world_size);

    // Проверка соответствия сумм
    ASSERT_EQ(sum, expected_sum);

    deleteMatrix(matrix, size);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}
