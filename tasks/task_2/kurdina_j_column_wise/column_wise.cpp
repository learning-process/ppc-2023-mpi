// Copyright 2023 Kurdina Julia
#include "task_2/kurdina_j_column_wise/column_wise.h"
#include <vector>
#include <random>

std::vector<int> get_rand_matrix(int columns, int rows, int l, int r) {
  int size = columns * rows;
  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_int_distribution<int> distrib(l, r);
  std::vector<int> M(size);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      M[i * columns + j] = distrib(gen);
    }
  }
  return M;
}

std::vector<int> transpose_M(const std::vector<int>& M, int columns, int rows) {
  std::vector<int> tM(M.size());
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      tM[i + j * rows] = M[i * columns + j];
    }
  }
  return tM;
}

std::vector<int> columnwise_sum(const std::vector<int>& M, const std::vector<int>& b, int rows, int columns) {
  std::vector<int> c(rows);
  for (int i = 0; i < rows; i++) {
    c[i] = 0;
    for (int j = 0; j < columns; j++) {
      c[i] += M[i * columns + j] * b[j];
    }
  }
  return c;
}

std::vector<int> columnwise_sum_parallel(const std::vector<int>& M, const std::vector<int>& b, int rows, int columns) {
  int procNum, procRank;
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  std::vector<int> sendcounts(procNum);
  std::vector<int> displs(procNum);
  std::vector<int> sendcounts_b(procNum);
  std::vector<int> displs_b(procNum);
  int ost = columns % procNum;
  int size = columns / procNum;
  std::vector<int> new_M(size * rows);
  std::vector<int> new_b(size);
  if (procRank == 0) {
    displs[0] = 0;
    displs_b[0] = 0;
    for (int i = 0; i < procNum; i++) {
      sendcounts[i] = rows * size;
      sendcounts_b[i] = size;
    }
    sendcounts[0] += ost * rows;
    sendcounts_b[0] += ost;
    for (int i = 1; i < procNum; i++) {
      displs[i] = displs[i - 1] + sendcounts[i - 1];
      displs_b[i] = displs_b[i - 1] + sendcounts_b[i - 1];
    }
    new_M.resize((ost + size) * rows);
    new_b.resize(ost + size);
  }
  std::vector<int> tM = transpose_M(M, columns, rows);
  MPI_Scatterv(tM.data(), sendcounts.data(), displs.data(), MPI_INT,
    new_M.data(), new_M.size(), MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatterv(b.data(), sendcounts_b.data(), displs_b.data(), MPI_INT,
    new_b.data(), new_b.size(), MPI_INT, 0, MPI_COMM_WORLD);
  std::vector<int> c_res(rows);
  std::vector<int> z(rows);
  int columnNum = new_M.size() / rows;
  for (int i = 0; i < columnNum; i++) {
    for (int j = 0; j < rows; j++) {
      z[j] += new_M[i * rows + j] * new_b[i];
    }
  }
  MPI_Reduce(z.data(), c_res.data(), rows, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  return c_res;
}
