// Copyright 2023 Mortina Anastasia
#include "task_2/mortina_a_horizontal_ribbon_scheme/horizontal_scheme.h"

int generateRandomNumbers(int min, int max) {
  int randomNumber = min + (std::rand() % (max - min + 1));

  return randomNumber;
}

std::vector<int> generateMatrixRandom(int rows, int cols, int min, int max) {
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

void matrixVectorMultiply(const std::vector<int>& matrix, const std::vector<int>& vector,
                                       std::vector<int>* result, int cols, int start, int end) {
    for (int i = start; i < end; i++) {
        int sum = 0;
        for (int j = 0; j < cols; j++) {
            sum += matrix[i * cols + j] * vector[j];
        }
       (*result)[i] = sum;
    }
}


std::vector<int> ParallMatrixVector(const std::vector<int>& matrix,
                            const std::vector<int>& vector, int rows, int cols) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> local_result(rows);

    int rowsPerProcess = rows / size;
    int start = rank * rowsPerProcess;
    int end = (rank == size - 1) ? rows : (rank + 1) * rowsPerProcess;

    matrixVectorMultiply(matrix, vector, &local_result, cols, start, end);

    std::vector<int> globalResult(rows);
    MPI_Allreduce(&local_result[0], &globalResult[0], rows, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    return globalResult;
}
