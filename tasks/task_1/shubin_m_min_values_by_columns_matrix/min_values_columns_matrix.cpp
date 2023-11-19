// Copyright 2023 Shubin Mikhail
#include "task_1/shubin_m_min_values_by_columns_matrix/min_values_columns_matrix.h"

std::vector<int> getRandomMatrix(size_t row, size_t col, int min_v, int max_v) {
  std::vector<int> matrix(row * col);
  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_int_distribution<> distrib(min_v, max_v);

  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      matrix[i * col + j] = distrib(gen);
    }
  }

  return matrix;
}

std::vector<int> getColumnMin_seq(const std::vector<int> &matr,
                                  size_t row, size_t col) {
  // Matrices are stored as rows in C++, so we need to transpose input matrix
  std::vector<int> trsp_matr(row * col);
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      trsp_matr[j * row + i] = matr[i * col + j];
    }
  }

  std::vector<int> min_v(col);
  int min = 0;

  for (int i = 0; i < col; i++) {
    min = std::numeric_limits<int>::max();
    for (int j = 0; j < row; j++) {
      min = std::min(min, trsp_matr[i * row + j]);
    }
    min_v[i] = min;
  }

  return min_v;
}

std::vector<int> getColumnMin_par(const std::vector<int> &matr,
                                  size_t row, size_t col) {
  // Matrices are stored as rows in C++, so we need to transpose input matrix
  std::vector<int> trsp_matr(row * col);
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      trsp_matr[j * row + i] = matr[i * col + j];
    }
  }

  int ProcRank, ProcNum;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int size = col / ProcNum,
      rem = col % ProcNum,
      min_vs = 0;
  std::vector<int> sendcnts(ProcNum);
  std::vector<int> recvcnts(ProcNum);
  std::vector<int> displs_1(ProcNum);
  std::vector<int> displs_2(ProcNum);
  std::vector<int> transm_col(size * row);
  std::vector<int> min_v(col);

  if (ProcRank == 0) {
    for (int i = 0; i < ProcNum; i++) {
      sendcnts[i] = size * row;
      displs_1[i] = 0;
      recvcnts[i] = size;
      displs_2[i] = 0;
    }
    sendcnts[0] += rem * row;
    recvcnts[0] += rem;
    for (int i = 1; i < ProcNum; i++) {
      displs_1[i] = displs_1[i - 1] + sendcnts[i - 1];
      displs_2[i] = displs_2[i - 1] + recvcnts[i - 1];
    }
    transm_col.resize((size + rem) * row);
  }

  MPI_Scatterv(trsp_matr.data(), sendcnts.data(), displs_1.data(), MPI_INT, transm_col.data(), transm_col.size(),
               MPI_INT, 0, MPI_COMM_WORLD);

  std::vector<int> res_vec;
  int k = transm_col.size() / row;
  for (int i = 0; i < k; i++) {
    min_vs = std::numeric_limits<int>::max();
    for (int j = 0; j < row; j++) {
      min_vs = std::min(min_vs, transm_col[i * row + j]);
    }
    res_vec.push_back(min_vs);
  }

  MPI_Gatherv(res_vec.data(), res_vec.size(), MPI_INT, min_v.data(), recvcnts.data(), displs_2.data(),
              MPI_INT, 0, MPI_COMM_WORLD);

  return min_v;
}
