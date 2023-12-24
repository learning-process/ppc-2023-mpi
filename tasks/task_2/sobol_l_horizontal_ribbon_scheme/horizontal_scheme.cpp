// Copyright 2023 Sobol Lubov

#include "task_2/sobol_l_horizontal_ribbon_scheme/horizontal_ribbon_scheme.h"


std::vector<int> generateRandomMatrix(int rows, int cols, int min, int max) {
    std::vector<int> matrix(rows * cols);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i * cols + j] = dist(gen);
        }
    }

    return matrix;
}

int generateSingleRandomNumber(int min, int max) {
  int randomNumber = min + (std::rand() % (max - min + 1));

  return randomNumber;
}

void multiplyMatrixVector(const std::vector<int>& matrix, const std::vector<int>& vector,
                                       std::vector<int>* result, int cols, int start, int end) {
    for (int i = start; i < end; i++) {
        int sum = 0;
        for (int j = 0; j < cols; j++) {
            sum += matrix[i * cols + j] * vector[j];
        }
       (*result)[i] = sum;
    }
}


std::vector<int> parallelMatrixVector(const std::vector<int>& matrix,
                            const std::vector<int>& vector, int rows, int cols) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> localResult(rows);

    int rowsPerProcess = rows / size;
    int end = (rank == size - 1) ? rows : (rank + 1) * rowsPerProcess;

    multiplyMatrixVector(matrix, vector, &localResult, cols, rank * rowsPerProcess, end);

    std::vector<int> globalResult(rows);
    MPI_Allreduce(&localResult[0], &globalResult[0], rows, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    return globalResult;
}
