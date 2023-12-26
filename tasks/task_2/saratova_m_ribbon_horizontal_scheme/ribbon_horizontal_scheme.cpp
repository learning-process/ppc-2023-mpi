// Copyright 2023 Saratova Marina
#include "task_2/saratova_m_ribbon_horizontal_scheme/ribbon_horizontal_scheme.h"

std::vector<std::vector<int>> generate_random_matrix(int rows,
             int cols, int lower_limit, int upper_limit) {
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(lower_limit, upper_limit);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = distribution(gen);
        }
    }
    return matrix;
}

bool compare_matrix(const std::vector<std::vector<int>>& matrix1,
                 const std::vector<std::vector<int>>& matrix2) {
    if (matrix1.size() != matrix2.size() ||
     matrix1[0].size() != matrix2[0].size()) {
        return false;
    }
    for (size_t i = 0; i < matrix1.size(); ++i) {
        for (size_t j = 0; j < matrix1[0].size(); ++j) {
            if (matrix1[i][j] != matrix2[i][j]) {
                return false;
            }
        }
    }
    return true;
}

std::vector<std::vector<int>> vector_to_matrix
                (const std::vector<int>& vector, int rows, int cols) {
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    int r = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = vector[r];
            r++;
        }
    }
    return matrix;
}

std::vector<int> matrix_to_vector
            (const std::vector<std::vector<int>>& matrix) {
    std::vector<int> vector(matrix.size() * matrix[0].size());
    int r = 0;
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            vector[r] = matrix[i][j];
            r++;
        }
    }
    return vector;
}

std::vector<std::vector<int>> seq_matrix_multiply
        (const std::vector<std::vector<int>>& matrix1,
        const std::vector<std::vector<int>>& matrix2) {
    int rows1 = matrix1.size();
    int cols1 = matrix1[0].size();
    int rows2 = matrix2.size();
    int cols2 = matrix2[0].size();
    if (cols1 != rows2) {
        return std::vector<std::vector<int>>();
    }
    std::vector<std::vector<int>> result(rows1, std::vector<int>(cols2, 0));
    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols2; ++j) {
            for (int k = 0; k < cols1; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}

std::vector<std::vector<int>> par_ribbon_horizontal_scheme
            (const std::vector<std::vector<int>>& matrix1,
            const std::vector<std::vector<int>>& matrix2) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (matrix1[0].size() != matrix2.size()) {
        return std::vector<std::vector<int>>();
    }

    const int rows1 = matrix1.size();
    const int cols1 = matrix1[0].size();
    const int rows2 = matrix2.size();
    const int cols2 = matrix2[0].size();

    int rows_per_process = rows1 / size;
    int remainder = rows1 % size;

    int local_rows = rows_per_process + (rank < remainder ? 1 : 0);

    std::vector<int> sendcounts(size);
    std::vector<int> displs(size);

    for (int i = 0; i < size; ++i) {
        sendcounts[i] = rows_per_process * cols1 +
                                     (i < remainder ? cols1 : 0);
        displs[i] = i * rows_per_process * cols1 +
                                 std::min(i, remainder) * cols1;
    }

    std::vector<int> matrix1_to_vector = matrix_to_vector(matrix1);
    std::vector<int> local_matrix1(local_rows * cols1);
    MPI_Scatterv(matrix1_to_vector.data(), sendcounts.data(),
                                                 displs.data(), MPI_INT,
                 local_matrix1.data(), local_rows * cols1, MPI_INT,
                 0, MPI_COMM_WORLD);
    std::vector<std::vector<int>> new_local_matrix1 = vector_to_matrix
                                    (local_matrix1, local_rows, cols1);

    std::vector<int> matrix2_to_vector = matrix_to_vector(matrix2);
    MPI_Bcast(matrix2_to_vector.data(), rows2 * cols2, MPI_INT,
                                            0, MPI_COMM_WORLD);
    std::vector<std::vector<int>> new_matrix2 = vector_to_matrix
                            (matrix2_to_vector, rows2, cols2);

    std::vector<std::vector<int>> local_result = seq_matrix_multiply
                                    (new_local_matrix1, new_matrix2);

    std::vector<int> local_result_vector = matrix_to_vector(local_result);
    std::vector<int> result(rows1 * cols2);
    MPI_Gatherv(local_result_vector.data(), local_result_vector.size(), MPI_INT,
                result.data(), sendcounts.data(), displs.data(), MPI_INT,
                0, MPI_COMM_WORLD);

    if (rank == 0) {
        return vector_to_matrix(result, rows1, cols2);
    }

    return std::vector<std::vector<int>>();
}
