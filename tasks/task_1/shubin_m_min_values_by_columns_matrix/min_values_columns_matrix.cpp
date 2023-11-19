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

  std::vector<int> min_v,
                   sendc,
                   displ,
                   loc_vec,
                   res_vec;
  MPI_Datatype datatype,
               column;
  int wsize = 0,
      rank = 0,
      chunk = 0,
      remainder = 0,
      min_vs = 0;

  MPI_Comm_size(MPI_COMM_WORLD, &wsize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  chunk = col / wsize;
  remainder = col % wsize;
  loc_vec.resize(chunk * row);

  if (rank == 0) {
    min_v.resize(col);
    sendc.resize(wsize, chunk);
    sendc[0] = sendc[0] + remainder;
    displ.resize(wsize, 0);
    for (int i = 1; i < wsize; i++)
      displ[i] = displ[i - 1] + sendc[i - 1];
    loc_vec.resize((chunk + remainder) * row);
  }

  MPI_Type_vector(col, 1, row, MPI_INT, &datatype);
  MPI_Type_commit(&datatype);
  MPI_Type_create_resized(datatype, 0, sizeof(int) * 1, &column);
  MPI_Type_commit(&column);

  MPI_Scatterv(trsp_matr.data(),
               sendc.data(), displ.data(),
               column,
               loc_vec.data(),
               static_cast<int>(loc_vec.size()),
               MPI_INT, 0, MPI_COMM_WORLD);

  int loc_size = loc_vec.size() / row;
  for (int i = 0; i < loc_size; i++) {
    min_vs = std::numeric_limits<int>::max();
    for (int j = 0; j < row; j++)
      min_vs = std::min(min_vs, loc_vec[i * row + j]);
    res_vec.push_back(min_vs);
  }

  MPI_Gatherv(res_vec.data(), res_vec.size(),
              MPI_INT, min_v.data(),
              sendc.data(), displ.data(),
              MPI_INT, 0, MPI_COMM_WORLD);

  return min_v;
}
