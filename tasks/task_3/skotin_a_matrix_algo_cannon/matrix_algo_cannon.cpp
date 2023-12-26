// Copyright 2023 Skotin Alexander
#include "task_3/skotin_a_matrix_algo_cannon/matrix_algo_cannon.h"

std::vector<std::vector<double>> GetMatrixRandom(int size) {
    std::vector<std::vector<double>> matrix(size, std::vector<double>(size));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = static_cast<double>(
                rand()) / RAND_MAX * 100.0 - 50.0;
        }
    }
    return matrix;
}

std::vector<std::vector<double>> MatrixMultiply(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B, int matrix_dimension) {
    std::vector<std::vector<double>> result(matrix_dimension,
        std::vector<double>(matrix_dimension, 0));
    for (int i = 0; i < matrix_dimension; i++) {
        for (int j = 0; j < matrix_dimension; j++) {
            for (int k = 0; k < matrix_dimension; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}
std::vector<std::vector<double>> CannonMultiply(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B, int matrix_dimension) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int q = std::sqrt(size);
    if (q * q != size || A.size() % q != 0 || B.size() % q != 0) {
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        return {};
    }

    int local_n = matrix_dimension / q;
    std::vector<double> local_A(local_n * local_n), local_B(local_n * local_n);
    std::vector<double> local_C(local_n * local_n, 0.0);

    MPI_Datatype blocktype, blocktype2;
    MPI_Type_vector(local_n, local_n, matrix_dimension, MPI_DOUBLE,
        &blocktype2);
    MPI_Type_create_resized(blocktype2, 0, sizeof(double), &blocktype);
    MPI_Type_commit(&blocktype);

    std::vector<double> matrixA, matrixB;
    if (rank == 0) {
        for (const auto& row : A)
            for (double val : row)
                matrixA.push_back(val);
        for (const auto& row : B)
            for (double val : row)
                matrixB.push_back(val);
    }

    matrixA.resize(matrix_dimension * matrix_dimension);
    matrixB.resize(matrix_dimension * matrix_dimension);

    int* counts = new int[size];
    int* displs = new int[size];
    for (int i = 0; i < size; ++i) counts[i] = 1;
    for (int i = 0; i < size; ++i) displs[i] = (i / q) * matrix_dimension
        * local_n + (i % q) * local_n;

    MPI_Scatterv(matrixA.data(), counts, displs, blocktype, local_A.data(),
        local_n * local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(matrixB.data(), counts, displs, blocktype, local_B.data(),
        local_n * local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int dims[2] = { q, q };
    int periods[2] = { 1, 1 };
    MPI_Comm grid_comm;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &grid_comm);

    int coords[2];
    MPI_Cart_coords(grid_comm, rank, 2, coords);

    int left, right, up, down;
    MPI_Cart_shift(grid_comm, 1, 1, &left, &right);
    MPI_Cart_shift(grid_comm, 0, 1, &up, &down);

    for (int step = 0; step < q; ++step) {
        for (int i = 0; i < local_n; ++i)
            for (int j = 0; j < local_n; ++j)
                for (int k = 0; k < local_n; ++k)
                    local_C[i * local_n + j] += local_A[i * local_n + k]
                    * local_B[k * local_n + j];

        MPI_Sendrecv_replace(local_A.data(), local_A.size(), MPI_DOUBLE, left,
            0, right, 0, grid_comm, MPI_STATUS_IGNORE);
        MPI_Sendrecv_replace(local_B.data(), local_B.size(), MPI_DOUBLE, up,
            0, down, 0, grid_comm, MPI_STATUS_IGNORE);
    }

    std::vector<double> gathered_C(matrix_dimension * matrix_dimension);
    MPI_Gatherv(local_C.data(), local_n * local_n, MPI_DOUBLE,
        gathered_C.data(), counts, displs, blocktype, 0, MPI_COMM_WORLD);

    MPI_Type_free(&blocktype);
    MPI_Type_free(&blocktype2);
    delete[] counts;
    delete[] displs;

    std::vector<std::vector<double>> C(matrix_dimension,
        std::vector<double>(matrix_dimension));
    if (rank == 0) {
        for (int i = 0; i < matrix_dimension; ++i)
            for (int j = 0; j < matrix_dimension; ++j)
                C[i][j] = gathered_C[i * matrix_dimension + j];
    }

    return C;
}
