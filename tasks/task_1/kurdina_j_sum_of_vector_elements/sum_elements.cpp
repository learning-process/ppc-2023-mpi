// Copyright 2023 Kurdina Julia
#include "task_1/kurdina_j_sum_of_vector_elements/sum_elements.h"
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

int get_sum_parallel(const std::vector<int>& M, int lines) {
  int procNum, procRank;
  int summa, sum_all;
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  std::vector<int> sendcounts(procNum);
  std::vector<int> displs(procNum);
  int size = M.size() / procNum;
  int ost = M.size() % procNum;
  std::vector<int> new_M(size * lines);
  if (procRank == 0) {
    displs[0] = 0;
    for (int i = 0; i < procNum; i++) {
      sendcounts[i] = size;
    }
    sendcounts[0] += ost;
    for (int i = 1; i < procNum; i++) {
      displs[i] = displs[i - 1] + sendcounts[i - 1];
    }
    new_M.resize((size + ost) * lines);
  }
  MPI_Scatterv(M.data(), sendcounts.data(), displs.data(), MPI_INT,
    new_M.data(), new_M.size(), MPI_INT, 0, MPI_COMM_WORLD);
  summa = 0;
  sum_all = 0;
  int count = new_M.size();
  for (int i = 0; i < count; i++) {
    summa += new_M[i];
  }
  MPI_Reduce(&summa, &sum_all, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  return sum_all;
}

int get_sum(const std::vector<int>& M) {
  int res = 0;
  for (int i = 0; i < M.size(); i++) {
    res += M[i];
  }
  return res;
}
