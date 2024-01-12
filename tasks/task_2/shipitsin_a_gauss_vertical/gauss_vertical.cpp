// Copyright 2023 Shipitsin Alex
#include <mpi.h>
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>
#include "task_2/shipitsin_a_gauss_vertical/gauss_vertical.h"

#define EPSILON 0.000001

Matrix RandomMatrix(int rows, int cols) {
    std::random_device rd;
    std::mt19937 mersenne(rd());
    std::uniform_real_distribution<> rnd(-30, 30);
    Matrix matrix(rows * cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i * cols + j] = static_cast<int>(rnd(mersenne));
        }
    }
    return matrix;
}

Matrix SequentialGauss(const Matrix& matrix, int rows, int cols, const Matrix& vec, int vector_size) {
    if ((rows != vector_size) || (rows != cols) || (rows == 0) || (vector_size == 0)) {
        throw("Invalid size");
    }
    Matrix temp_matrix(rows, cols);
    Matrix temp_vector(rows);
    temp_vector = vec;
    temp_matrix = matrix;
    std::vector<int> pivot_order(rows);
    Matrix coefs(rows);
    Matrix result(rows);
    std::vector<bool> was_pivot(rows);
    std::fill(was_pivot.begin(), was_pivot.end(), false);
    double d;
    // Прямой ход
    for (int current_col = 0; current_col < cols - 1; current_col++) {
        // Найти максимум по модулю в столбце
        double max = 0.0;
        int max_i = 0;
        for (int i = 0; i < rows; i++) {
            if ((fabs(temp_matrix[i * cols + current_col]) >= fabs(max))&&(!was_pivot[i])) {
                max = temp_matrix[i * cols + current_col];
                max_i = i;
            }
        }
        was_pivot[max_i] = true;
        pivot_order[current_col] = max_i;
        // Подсчитать коэффиценты
        for (int k = 0; k < rows; k++) {
            coefs[k] = 0.0;
            if (!was_pivot[k]) {
                coefs[k] = temp_matrix[k * cols + current_col] /
                            temp_matrix[max_i * cols + current_col];
            }
        }
        // Вычесть ведущую строку из остальных
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                if (!was_pivot[row]) {
                    temp_matrix[row * cols + col] -= temp_matrix[max_i * cols + col] *
                        coefs[row];
                }
            }
            // Повторить для вектора b
            if (!was_pivot[row]) {
                temp_vector[row] -= temp_vector[max_i] * coefs[row];
            }
        }
    }
    // Добавить последнюю неиспользованную строку в последовательность
    //  (нужно для обратного хода)
    for (int i = 0; i < rows; i++) {
        if (!was_pivot[i]) {
            pivot_order[rows - 1] = i;
            break;
        }
    }
    // Обратный ход
    for (int n = rows - 1; n >= 0 ; n--) {
        d = 0.0;
        for (int k = n + 1; k < rows; k++) {
            d += result[k] *
                temp_matrix[pivot_order[n] * cols + k];
        }
        result[n] = (temp_vector[pivot_order[n]] - d) /
            temp_matrix[pivot_order[n] * cols + n];
    }
    return result;
}

Matrix ParallelGauss(const Matrix& matrix, int rows, int cols,
    const Matrix& vec, int vector_size) {
    if ((rows != vector_size) || (rows != cols) || (rows == 0) || (vector_size == 0)) {
        throw("Invalid size");
    }
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    Matrix temp_vector(rows);
    temp_vector = vec;
    if (procNum == 1) {
        return SequentialGauss(matrix, rows, cols, vec, vector_size);
    }
    const int delta = cols / procNum;
    const int remain = cols % procNum;
    int remain_for_proc;
    int *remains = new int[procNum];
    if (procRank == 0) {
        for (int proc = 0; proc < procNum; proc++) {
            remains[proc] = (proc < remain ? 1 : 0);
        }
    }
    MPI_Scatter(remains, 1, MPI_INT, &remain_for_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
    Matrix local_matrix((delta + remain_for_proc) * rows);
    Matrix coefs(rows);
    int *pivot_order = new int[rows];
    char *was_pivot = new char[rows];
    Matrix result(rows);
    for (int row = 0; row < rows; row++) {
        pivot_order[row] = -1;
        was_pivot[row] = -1;
    }
    // Создать тип данных - столбец
    MPI_Datatype Column;
    MPI_Type_vector(rows, 1, rows, MPI_DOUBLE, &Column);
    MPI_Type_commit(&Column);
    // Передача данных и прием данных
    if (procRank == 0) {
        for (int col = 0; col < cols; col++) {
            if (col % procNum != 0) {
                MPI_Send(matrix.data() + col, 1, Column, (col % procNum), 0, MPI_COMM_WORLD);
            }
        }
        // local_matrix для procRank == 0
        for (int row = 0; row < rows; row++) {
            for (int col_count = 0; col_count < delta + remain_for_proc; col_count++) {
                local_matrix[col_count * cols + row] = matrix[row * cols + col_count * procNum];
            }
        }
    } else {
        MPI_Status status;
        for (int row_count = 0; row_count < delta + remain_for_proc; row_count++) {
            MPI_Recv(local_matrix.data() + row_count * rows, rows, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);

    int root, new_root;
    // Прямой ход
    for (int current_col = 0; current_col < cols - 1; current_col++) {
        new_root = -1;
        root = -1;
        if (current_col % procNum == procRank) {
            new_root = procRank;
            // Найти максимум по модулю в столбце
            double max = 0.0;
            int max_i = 0;
            for (int local_col = 0; local_col < rows; local_col++) {
                if ((fabs(local_matrix[(current_col / procNum) * rows + local_col]) >= fabs(max))&&
                    (was_pivot[local_col] == -1)) {
                    max = local_matrix[(current_col / procNum) * rows + local_col];
                    max_i = local_col;
                }
            }
            was_pivot[max_i] = 1;
            pivot_order[current_col] = max_i;
            // Подсчитать коэффиценты
            for (int k = 0; k < rows; k++) {
                coefs[k] = 0.0;
                if (was_pivot[k] == -1) {
                    coefs[k] = local_matrix[(current_col / procNum) * rows + k] /
                                local_matrix[(current_col / procNum) * rows + max_i];
                }
            }
        }
        // Передать root, was_pivot, coefs, pivot_order
        MPI_Allreduce(&new_root, &root, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
        MPI_Bcast(was_pivot, rows, MPI_CHAR, root, MPI_COMM_WORLD);
        MPI_Bcast(coefs.data(), rows, MPI_DOUBLE, root, MPI_COMM_WORLD);
        MPI_Bcast(pivot_order, rows, MPI_INT, root, MPI_COMM_WORLD);
        // Вычесть ведущую строку из остальных
        for (int local_row = 0; local_row < delta + remain_for_proc; local_row++) {
            for (int local_col = 0; local_col < rows; local_col++) {
                if ((was_pivot[local_col] == -1) || (fabs(local_matrix[local_row* rows + local_col]) < EPSILON)) {
                    local_matrix[local_row* rows + local_col] -=
                    coefs[local_col] *
                    local_matrix[local_row * rows + pivot_order[current_col]];
                    // Отсечь значения, меньшие по модулю, чем эпсилон
                    local_matrix[local_row* rows + local_col] =
                    (fabs(local_matrix[local_row* rows + local_col]) < EPSILON ?
                    0 : local_matrix[local_row* rows + local_col]);
                }
            }
        }
        // Повторить для вектора b
        if (procRank == 0) {
            for (int row = 0; row < rows; row++) {
                if (was_pivot[row] == -1) {
                    temp_vector[row] -= temp_vector[pivot_order[current_col]] * coefs[row];
                }
            }
        }
    }
    // Добавить последнюю неиспользованную строку в последовательность
    //  (нужно для обратного хода)
    for (int i = 0; i < rows; i++) {
        if (was_pivot[i] == -1) {
            pivot_order[rows - 1] = i;
            break;
        }
    }
    // Обратный ход
    double d;
    if (procRank == 0) {
        Matrix triangle_transposed(cols * rows);
        for (int col = 0; col < cols; col++) {
            if (col % procNum == 0) {
                for (int row = 0; row < rows; row++) {
                    triangle_transposed[col * rows + row] =
                    local_matrix[(col / procNum) * rows + row];
                }
            } else {
                MPI_Status status;
                MPI_Recv(triangle_transposed.data() + col * rows, rows,
                MPI_DOUBLE, col % procNum, 0, MPI_COMM_WORLD, &status);
            }
        }
        for (int n = rows - 1; n >= 0 ; n--) {
        d = 0.0;
            for (int k = n + 1; k < rows; k++) {
                d += result[k] * triangle_transposed[k * cols + pivot_order[n]];
            }
            result[n] = (temp_vector[pivot_order[n]] - d) /
                triangle_transposed[n * cols + pivot_order[n]];
        }
    } else {
        for (int col_count = 0; col_count < delta + remain_for_proc; col_count++) {
           MPI_Send(local_matrix.data() + col_count * rows, rows, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
       }
    }
    delete[] was_pivot;
    delete[] remains;
    delete[] pivot_order;
    MPI_Type_free(&Column);
    return result;
}
