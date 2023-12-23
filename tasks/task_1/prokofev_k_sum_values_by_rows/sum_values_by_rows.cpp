// Copyright 2023 Prokofev Kirill
#include "task_1/prokofev_k_sum_values_by_rows/sum_values_by_rows.h"

int Random(int low, int high) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(low, high);
  return dist(gen);
}

std::vector<int> GenerateRandomMatrix(int n, int m) {
  if (n <= 0 || m <= 0)
    throw "Error";
  std::vector<int> newMatrix(n * m);
  for (int i = 0; i < n * m; i++)
    newMatrix.push_back(Random(-100, 100));
  return newMatrix;
}

std::vector<int> ParallSumValuesByRows(const std::vector<int>& matrix, int n, int m) {
  int numProc;
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  int rankProc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  int remainder = n % numProc;
  int rowsForOneProc = n / numProc;
  std::vector<int> localMatrix(rowsForOneProc * m);
  MPI_Scatter(matrix.data(), rowsForOneProc * m, MPI_INT,
    localMatrix.data(), rowsForOneProc * m, MPI_INT, 0, MPI_COMM_WORLD);
  std::vector<int> localSum = SeqSumValuesByRows(localMatrix, rowsForOneProc, m);
  std::vector<int>globalSum(n);
  if (rankProc == 0) {
    for (int i = 0; i < n; i++)
      globalSum[i] = 0;
  }
  MPI_Gather(localSum.data(), rowsForOneProc, MPI_INT, globalSum.data(), rowsForOneProc, MPI_INT, 0, MPI_COMM_WORLD);
  if (rankProc == 0) {
    for (int i = 0; i < remainder; i++) {
      for (int j = 0; j < m; j++)
        globalSum[rowsForOneProc * numProc + i] += matrix[(rowsForOneProc * numProc + i) * m + j];
    }
  }
  return globalSum;
}

std::vector<int> SeqSumValuesByRows(const std::vector<int>& matrix, int n, int m) {
  std::vector<int> locSum(n);
  int s;
  for (int i = 0; i < n; i++) {
    s = 0;
    for (int j = 0; j < m; j++)
      s += matrix[i * m + j];
    locSum.push_back(s);
  }
  return locSum;
}

