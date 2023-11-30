// Copyright 2023 Veselov Ilya
#include <mpi.h>
#include <random>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/serialization/vector.hpp>
#include "task_2/veselov_i_strip_hor_mult_matrix_vector/stripHorMultMatrixVector.h"

// std::vector<int> matrix_vector_multiply_par(std::vector<int> A, std::vector<int> x, int rows, int cols) {
std::vector<int> matrix_vector_multiply_par(std::vector<std::vector<int>> matrix, std::vector<int> vec,
 int rows, int cols, int rank, int rowsPerProcess, int extraRows) {
    /*boost::mpi::communicator world;
    std::vector<int> C(cols);
    int rank = world.rank(), p = world.size(), c_i;
    int prev = (p + rank - 1) % p, next = (rank + 1) % p, delta_rows = rows / p;
    std::vector<int> A_rows(delta_rows * cols), C_row(delta_rows);

    boost::mpi::scatter(world, A, A_rows.data(), cols * delta_rows, 0);
    boost::mpi::scatter(world, x, C.data(), cols, 0);

    if (world.rank() == 0) {
        A_rows = std::vector<int>(A.begin(), A.begin() + cols * delta_rows);
    }

    for (int dx = 0; dx < delta_rows; dx++) {
        c_i = (p + rank - 0) % p;
        c_i = c_i * delta_rows + dx;
        C_row[dx] = 0;
        for (int j = 0; j < cols; j++) C_row[dx] += A_rows[j + dx * cols] * C[j];
    }

    for (int i = 1; i < p; i++) {
        world.send(next, rank + p * i, C);
        world.recv(prev, prev + p * i, C);

        for (int dx = 0; dx < delta_rows; dx++) {
            c_i = (p + rank - i) % p;
            c_i = c_i * delta_rows + dx;
            C_row[dx] = 0;
            for (int j = 0; j < cols; j++) C_row[dx] += A_rows[j + dx * cols] * C[j];
        }
    }
    boost::mpi::gather(world, C_row.data(), delta_rows, A_rows.data(), 0);
    return A_rows;*/
    /*int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<int> C(cols);
    int p = size, c_i;
    int prev = (p + rank - 1) % p;
    int next = (rank + 1) % p;
    int delta_rows = rows / p;
    std::vector<int> A_rows(delta_rows * cols), C_row(delta_rows);
    MPI_Scatter(A.data(), cols * delta_rows, MPI_INT, A_rows.data(), cols * delta_rows, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(x.data(), cols, MPI_INT, C.data(), cols, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        A_rows = std::vector<int>(A.begin(), A.begin() + cols * delta_rows);
    }
    for (int dx = 0; dx < delta_rows; dx++) {
        c_i = (p + rank - 0) % p;
        c_i = c_i * delta_rows + dx;
        C_row[dx] = 0;
        for (int j = 0; j < cols; j++)
            C_row[dx] += A_rows[j + dx * cols] * C[j];
    }
    for (int i = 1; i < p; i++) {
        MPI_Sendrecv_replace(C.data(), cols, MPI_INT, next, rank + p * i, prev, prev + p * i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int dx = 0; dx < delta_rows; dx++) {
            c_i = (p + rank - i) % p;
            c_i = c_i * delta_rows + dx;
            C_row[dx] = 0;
            for (int j = 0; j < cols; j++)
                C_row[dx] += A_rows[j + dx * cols] * C[j];
        }
    }
    MPI_Gather(C_row.data(), delta_rows, MPI_INT, A_rows.data(), delta_rows, MPI_INT, 0, MPI_COMM_WORLD);
    return A_rows;*/

    std::vector<int> res(rows, 0);
    // int rowsPerProcess = rows / (size - 1);
    // int extraRows = rows % (size - 1);
    // int vecSize = cols;
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        for (int dest = 1; dest < size; ++dest) {
            int startRow = (dest - 1) * rowsPerProcess + 1;
            int numRows = rowsPerProcess;
            if (dest <= extraRows)
                numRows++;
        MPI_Send(&matrix[startRow - 1][0], numRows * cols, MPI_INT, dest, 0, MPI_COMM_WORLD);
        MPI_Send(&vec[0], cols, MPI_INT, dest, 0, MPI_COMM_WORLD);
        }
        for (int row = 1; row <= rowsPerProcess + extraRows; ++row) {
            std::vector<int> partRes(cols, 0);
            for (int col = 0; col < cols; ++col)
                partRes[row - 1] += matrix[row -1][col] * vec[col];
            res[row - 1] = partRes[row - 1];
        }
        for (int src = 1; src < size; ++src) {
            std::vector<int> recvRes(rowsPerProcess);
            MPI_Recv(&recvRes[0], rowsPerProcess, MPI_INT, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int i = 0; i < rowsPerProcess; ++i)
                res[(src - 1) * rowsPerProcess + i] = recvRes[i];
        }
    } else {
        int startRow = (rank - 1) * rowsPerProcess + 1;
        int numRows = rowsPerProcess;
        if (rank <= extraRows)
            numRows++;
        std::vector<std::vector<int>> subMatrix(numRows, std::vector<int>(cols));
        MPI_Recv(&subMatrix[0][0], numRows * cols, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&vec[0], cols, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::vector<int> partRes(numRows, 0);
        for (int i = 0; i < numRows; ++i)
            for (int j = 0; j < cols; ++j)
                partRes[i] += subMatrix[i][j] * vec[j];
        MPI_Send(&partRes[0], numRows, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    return res;
}

std::vector<int> matrix_vector_multiply(std::vector<std::vector<int>> matrix, std::vector<int> vec,
 int rows, int cols) {
    std::vector<int> res(rows);
    for (int i = 0; i < rows; ++i) {
        res[i] = 0;
        for (int j = 0; j < cols; ++j)
            res[i] += matrix[i][j] * vec[j];
    }
    return res;
}

/*std::vector<int> matrix_vector_multiply(std::vector<int> A, std::vector<int> x, int rows, int cols) {
    std::vector<int> res(rows);
    for (int i = 0; i < rows; i++) {
        res[i] = 0;
        for (int j = 0; j < cols; j++)
            res[i] += A[i * cols + j] * x[j];
    }
    return res;
}*/

std::vector<std::vector<int>> randomMatrix(int n, int m, int minElem, int maxElem) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minElem, maxElem);
    std::vector<std::vector<int>> matrix(n, std::vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            matrix[i][j] = dis(gen);
    return matrix;
}

/*std::vector<int> randomMatrix(size_t n, size_t m, int minElem, int maxElem) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minElem, maxElem);
    std::vector<int> matrix(n * m);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j)
            matrix[i * m + j] = dis(gen);
    }
    return matrix;
}*/

std::vector<int> randomVector(size_t n, int minElem, int maxElem) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minElem, maxElem);
    std::vector<int> vec(n);
    for (int i = 0; i < n; i++) {
        vec[i] = dis(gen);
    }
    return vec;
}
