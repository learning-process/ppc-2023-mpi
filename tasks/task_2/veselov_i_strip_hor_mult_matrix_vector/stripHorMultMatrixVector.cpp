// Copyright 2023 Veselov Ilya
#include <mpi.h>
#include <random>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/serialization/vector.hpp>
#include "task_2/veselov_i_strip_hor_mult_matrix_vector/stripHorMultMatrixVector.h"

/*std::vector<int> matrix_vector_multiply_par(std::vector<std::vector<int>> matrix, std::vector<int> vec,
 int rows, int cols, int rank, int rowsPerProcess, int extraRows) {

    std::vector<int> res(rows, 0);
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
}*/

/*std::vector<int> matrix_vector_multiply(std::vector<std::vector<int>> matrix, std::vector<int> vec,
 int rows, int cols) {
    std::vector<int> res(rows);
    for (int i = 0; i < rows; ++i) {
        res[i] = 0;
        for (int j = 0; j < cols; ++j)
            res[i] += matrix[i][j] * vec[j];
    }
    return res;
}*/

/*void matrix_vector_multiply(int* A, int* X, int* res, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        res[i] = 0;
        for (int j = 0; j < cols; j++)
            res[i] += A[i * cols + j] * X[j];
    }
}*/

std::vector<int> matrix_vector_multiply(const std::vector<int>& A, const std::vector<int>& X, int rows, int cols) {
    std::vector<int> res(rows);
    for (int i = 0; i < rows; i++) {
        res[i] = 0;
        for (int j = 0; j < cols; j++)
            res[i] += A[i * cols + j] * X[j];
    }
    return res;
}

/*std::vector<std::vector<int>> randomMatrix(int n, int m, int minElem, int maxElem) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minElem, maxElem);
    std::vector<std::vector<int>> matrix(n, std::vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            matrix[i][j] = dis(gen);
    return matrix;
}*/

/*void randomMatrix(int* matrix, int rows, int cols, int minElem, int maxElem) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minElem, maxElem);
    matrix = new int[rows * cols];
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            matrix[i * cols + j] = dis(gen);
}*/

std::vector<int> randomMatrix(int rows, int cols, int minElem, int maxElem) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minElem, maxElem);
    std::vector<int> matrix(rows * cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            matrix[i * cols + j] = dis(gen);
    return matrix;
}

/*int *buffA, *buffX, *buffRes;
    int rowA = rows / size;
    int rRes = rowA;
    int partA = rowA * rows;
    int partX = cols;
    int partRes = rRes;
    buffA = new int[partA];
    buffX = new int[partX];
    buffRes = new int[partRes];
    for (int i = 0; i < partRes; i++)
        buffRes[i] = 0;
    MPI_Scatter(A, partA, MPI_INT, buffA, partA, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(X, partX, MPI_INT, buffX, partX, MPI_INT, 0, MPI_COMM_WORLD);
    //MPI_Bcast(X, partX, MPI_INT, 0, MPI_COMM_WORLD);
    int k = 0, tmp = 0;
    MPI_Status status;
    for (int p = 0; p < size; p++) {
        for (int i = 0; i < rowA; i++) {
            tmp = 0;
            for (int j = 0; j < cols; j++)
                tmp += buffA[i * cols + j] * X[j];
            buffRes[i] += tmp;
        }
        MPI_Barrier(MPI_COMM_WORLD);
        //MPI_Sendrecv_replace(X, partX, MPI_INT, (rank + 1) % size, 0, (rank - 1 + size) % size, 0, MPI_COMM_WORLD, &status);
    }
    MPI_Gather(buffRes, partRes, MPI_INT, res, partRes, MPI_INT, 0, MPI_COMM_WORLD);
    delete[] buffA;
    delete[] buffX;
    delete[] buffRes;*/

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


/*void matrix_vector_multiply_par(int* A, int* X, int* res, int rows, int cols, int rank, int size) {
    int *buffA, *buffRes;
    int rowsPerProc = rows / size;
    int rowsForLastProc = rows - rowsPerProc * (size - 1);
    int partA = (rank == size - 1) ? rowsForLastProc * cols : rowsPerProc * cols;
    int partRes = (rank == size - 1) ? rowsForLastProc : rowsPerProc;
    //int partX = cols;
    buffA = new int[partA];
    //buffX = new int[partX];
    buffRes = new int[partRes];
    MPI_Scatter(A, partA, MPI_INT, buffA, partA, MPI_INT, 0, MPI_COMM_WORLD);
    //MPI_Scatter(X, partX, MPI_INT, buffX, partX, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(X, cols, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < partRes; i++) { //rowsPerProc
        buffRes[i] = 0;
        for (int j = 0; j < cols; j++) {
            buffRes[i] += buffA[i * cols + j] * X[j];
        }
    }
    //MPI_Gather(buffRes, rowsPerProc, MPI_INT, res, rowsPerProc, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(buffRes, partRes, MPI_INT, res, partRes, MPI_INT, 0, MPI_COMM_WORLD);
    delete[] buffA;
    //delete[] buffX;
    delete[] buffRes;
}*/


std::vector<int> matrix_vector_multiply_par(const std::vector<int>& A, const std::vector<int>& X, int rows, int cols) {
    // более-менее работает
    /*int rowsPerProc = rows / size;
    int rowsForLastProc = rows - rowsPerProc * (size - 1);
    int partA = (rank == size - 1) ? rowsForLastProc * cols : rowsPerProc * cols;
    int partRes = (rank == size - 1) ? rowsForLastProc : rowsPerProc;
    std::vector<int> buffA(partA);
    std::vector<int> buffRes(partRes);
    MPI_Scatter(A.data(), partA, MPI_INT, buffA.data(), partA, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(X.data(), cols, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < partRes; i++) {
        int tmp = 0;
        for (int j = 0; j < cols; j++) {
            tmp += buffA[i * cols + j] * X[j];
        }
        buffRes[i] = tmp;
    }
    MPI_Gather(buffRes.data(), partRes, MPI_INT, res.data(), partRes, MPI_INT, 0, MPI_COMM_WORLD);*/
    /*int rowsPerProc = rows / size;
    int rowsForLastProc = rows - rowsPerProc * (size - 1);
    std::vector<int> buffA, buffX, buffRes;
    if (rank == 0) {
        buffA = A;
        buffX = X;
        //buffA.resize(cols * rowsPerProc * size);
        //buffX.resize(cols * size);
    } else {
        buffA.resize(cols * rowsPerProc);
        buffX.resize(cols);
    }
    buffA.resize(cols * rowsPerProc);
    buffX.resize(cols);
    buffRes.resize(rowsPerProc);
    //MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatter(A.data(), cols * rowsPerProc, MPI_INT, buffA.data(), cols * rowsPerProc, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(X.data(), cols, MPI_INT, buffX.data(), cols, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < rowsPerProc; i++) {
        buffRes[i] = 0;
        for (int j = 0; j < cols; j++) {
            buffRes[i] += buffA[i * cols + j] * buffX[j];
        }
    }
    MPI_Gather(buffRes.data(), rowsPerProc, MPI_INT, res.data(), rowsPerProc, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0 && rowsForLastProc > 0) {
        int lastProcStart = rows - rowsForLastProc;
        std::fill(buffRes.begin(), buffRes.end(), 0);
        for (int i = 0; i < rowsForLastProc; i++) {
            //res[lastProcStart + i] = 0;
            for (int j = 0; j < cols; j++) {
                //res[lastProcStart + i] += A[(lastProcStart + i) * cols + j] * X[j];
                buffRes[i] += A[(lastProcStart + i) * cols + j] * X[j];
            }
        }
        std::copy(buffRes.begin(), buffRes.begin() + rowsForLastProc, res.begin() + lastProcStart);
    }*/
    /*std::vector<int> buffA, buffX, buffRes;
    int rowsPerProc = rows / size;
    int rowsForLastProc = rows - rowsPerProc * (size - 1);
    buffA.resize(cols * rowsPerProc);
    buffX.resize(cols);
    buffRes.resize(rowsPerProc);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatter(A.data(), cols * rowsPerProc, MPI_INT, buffA.data(), cols * rowsPerProc, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(X.data(), cols, MPI_INT, buffX.data(), cols, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < rowsPerProc; i++) {
        buffRes[i] = 0;
        for (int j = 0; j < cols; j++) {
            buffRes[i] += buffA[i * cols + j] * buffX[j];
        }
    }
    MPI_Gather(buffRes.data(), rowsPerProc, MPI_INT, res.data(), rowsPerProc, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0 && rowsForLastProc > 0) {
        int lastProcStart = cols - rowsForLastProc;
        buffA.resize(cols * rowsForLastProc);
        MPI_Scatter(A.data() + lastProcStart * cols, cols * rowsForLastProc, MPI_INT, buffA.data(), cols * rowsForLastProc, MPI_INT, 0, MPI_COMM_WORLD);
        for (int i = 0; i < rowsForLastProc; i++) {
            buffRes[i] = 0;
            for (int j = 0; j < cols; j++) {
                buffRes[i] += buffA[i * cols + j] * buffX[j];
            }
        }
        MPI_Gather(buffRes.data(), rowsForLastProc, MPI_INT, res.data() + lastProcStart, rowsForLastProc, MPI_INT, 0, MPI_COMM_WORLD);
    }*/
    /*int r, p;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    int delta = rows / p;
    std::vector<int> Arow(rows * delta), resRow(rows * delta);
    MPI_Scatter(A.data(), rows * delta, MPI_INT, Arow.data(), rows * delta, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(X.data(), delta, MPI_INT, X.data(), delta, MPI_INT, 0, MPI_COMM_WORLD);
    if (r == 0) {
        Arow = std::vector<int>(A.begin(), A.begin() + rows * delta);
    }
    for (int dx = 0; dx < delta; dx++) {
        int res_i = (p + r - 0) % p;
        res_i = res_i * delta + dx;
        resRow[res_i] = 0;
        for (int j = 0; j < rows; j++)
            resRow[res_i] += Arow[j] * X[dx];
    }
    for (int i = 1; i < p; i++) {
        MPI_Send(X.data(), delta, MPI_INT, (r + i) % p, r + p * i, MPI_COMM_WORLD);
        MPI_Recv(X.data(), delta, MPI_INT, (r + p - i) % p, (r + p * i) % p, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int dx = 0; dx < delta; dx++) {
            int res_i = (p + r - i) % p;
            res_i = res_i * delta + dx;
            resRow[res_i] = 0;
            for (int j = 0; j < rows; j++)
                resRow[res_i] += Arow[j] * X[dx];
        }
    }
    MPI_Gather(resRow.data(), rows * delta, MPI_INT, res.data(), rows * delta, MPI_INT, 0, MPI_COMM_WORLD);*/
    /*std::vector<int> buffA, buffX, buffRes;
    int rowsPerProcA = rows / size;
    int rowsPerProcX = cols / size;
    int rowsPerProcRes = rowsPerProcA;
    int partA = rowsPerProcA * cols;
    int partX = rowsPerProcX;
    int partRes = rowsPerProcRes;
    buffA.resize(partA);
    buffX.resize(partX);
    buffRes.resize(partRes);
    for (int i = 0; i < partRes; i++)
        buffRes[i] = 0;
    MPI_Scatter(A.data(), partA, MPI_INT, buffA.data(), partA, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(X.data(), partX, MPI_INT, buffX.data(), partX, MPI_INT, 0, MPI_COMM_WORLD);
    //int nextProc = rank + 1;
    //if (rank == size - 1)
    //    nextProc = 0;
    //int prevProc = rank - 1;
    //if (rank == 0)
    //    prevProc = size - 1;
    int nextProc = (rank + 1) % size;
    int prevProc = (rank + size - 1) % size;
    MPI_Status status;
    for (int p = 0; p < size; p++) {
        for (int i = 0; i < rowsPerProcA; i++) {
            for (int j = 0; j < rowsPerProcX; j++)  {
                buffRes[i] += buffA[i * cols + j] * buffX[j];
            }
        }
        MPI_Sendrecv_replace(buffX.data(), partX, MPI_INT, nextProc, 0, prevProc, 0, MPI_COMM_WORLD, &status);
    }
    MPI_Gather(buffRes.data(), partRes, MPI_INT, res.data(), partRes, MPI_INT, 0, MPI_COMM_WORLD);
    buffA.clear();
    buffX.clear();
    buffRes.clear();*/
    /*std::vector<int> buffA, buffX, buffRes;
    int rowsPerProcA = rows / size;
    int rowsPerProcX = cols / size;
    int rowsPerProcRes = rowsPerProcA;
    int partA = rowsPerProcA * cols;
    int partX = rowsPerProcX * cols;
    int partRes = rowsPerProcRes;
    buffA.resize(partA);
    buffX.resize(partX);
    buffRes.resize(partRes);
    for (int i = 0; i < partRes; i++)
        buffRes[i] = 0;
    MPI_Scatter(A.data(), partA, MPI_INT, buffA.data(), partA, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(X.data(), partX, MPI_INT, buffX.data(), partX, MPI_INT, 0, MPI_COMM_WORLD);
    int nextProc = rank + 1;
    if (rank == size - 1)
        nextProc = 0;
    int prevProc = rank - 1;
    if (rank == 0)
        prevProc = size - 1;
    int tmp;
    MPI_Status status;
    int prevDataNum = rank;
    for (int p = 0; p < size; p++) {
        for (int i = 0; i < rowsPerProcA; i++) {
            tmp = 0;
            for (int j = 0; j < cols; j++) {
                tmp += buffA[i * cols + j] * buffX[j];
            }
            buffRes[i] += tmp;
        }
        prevDataNum -= 1;
        if (prevDataNum < 0)
            prevDataNum = size - 1;
        MPI_Sendrecv_replace(buffX.data(), partX, MPI_INT, nextProc, 0, prevProc, 0, MPI_COMM_WORLD, &status);
    }
    MPI_Gather(buffRes.data(), partRes, MPI_INT, res.data(), partRes, MPI_INT, 0, MPI_COMM_WORLD);*/
    /*int rowsPerProc = rows / size;
    int leftOverRows = rows % size;
    int startRow = rank * rowsPerProc;
    int endRow = startRow + rowsPerProc - 1;
    if (rank == size - 1) {
        endRow += leftOverRows;
    }
    std::vector<int> localRes(rowsPerProc, 0);
    for (int i = startRow; i <= endRow; ++i) {
        for (int j = 0; j < cols; ++j) {
            localRes[i - startRow] += A[i * cols + j] * X[j];
        }
    }
    MPI_Gather(localRes.data(), rowsPerProc, MPI_INT, res.data(), rowsPerProc, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == size - 1 && leftOverRows > 0) {
        std::vector<int> extraRes(leftOverRows);
        for (int i = startRow + rowsPerProc; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                extraRes[i - startRow - rowsPerProc] += A[i * cols + j] * X[j];
            }
        }
        MPI_Gather(extraRes.data(), leftOverRows, MPI_INT, res.data() + startRow * rowsPerProc, leftOverRows, MPI_INT, 0, MPI_COMM_WORLD);
    }*/
    int r, s;
    std::vector<int> result(rows, 0);
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Comm_size(MPI_COMM_WORLD, &s);
    int chunkSize = rows / s;
    int remainder = rows % s;
    int start = r * chunkSize;
    int end = (r + 1) * chunkSize;
    if (r == s - 1)
        end += remainder;
    std::vector<int> localRes(chunkSize, 0);
    std::vector<int> localMatrix(chunkSize * cols);
    MPI_Scatter(A.data(), chunkSize * cols, MPI_INT, localMatrix.data(), chunkSize * cols, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(X.data(), cols, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = start; i < end; ++i) {
        for (int j = 0; j < cols; ++j) {
            localRes[i - start] += localMatrix[(i - start) * cols + j] * X[j];
        }
    }
    MPI_Gather(localRes.data(), chunkSize, MPI_INT, result.data(), chunkSize, MPI_INT, 0, MPI_COMM_WORLD);
    return result;
}

/*void randomVector(int* vec, int size, int minElem, int maxElem) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minElem, maxElem);
    vec = new int[size];
    for (int i = 0; i < size; i++)
        vec[i] = dis(gen);
}*/

std::vector<int> randomVector(int size, int minElem, int maxElem) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minElem, maxElem);
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++)
        vec[i] = dis(gen);
    return vec;
}

/*std::vector<int> randomVector(size_t n, int minElem, int maxElem) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minElem, maxElem);
    std::vector<int> vec(n);
    for (int i = 0; i < n; i++) {
        vec[i] = dis(gen);
    }
    return vec;
}*/
