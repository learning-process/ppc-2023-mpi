// Copyright 2023 Shmelev Ivan
#include "task_1/shmelev_i_max_of_matrix_elements/max_of_matrix_elements.h"

int MaxInMatrixSeq(const std::vector<int>& matrix) {
  if (matrix.empty()) return 0;
  int max_value = INT_MIN;

  for (const auto& val : matrix)
    if (val > max_value) max_value = val;

  return max_value;
}

int MaxInMatrixPar(const std::vector<int>& matrix) {
  if (matrix.empty()) return 0;

  size_t size_matr = matrix.size();
  int rank = 0;
  int size = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int delta = size_matr / size;
  int remainainder = size_matr % size;

  std::vector<int> send_count_elem(size, delta);
  std::vector<int> offset(size);

  for (int i = 0; i < size; ++i) {
    if (i < remainainder) ++send_count_elem[i];
    offset[i] = i == 0 ? 0 : send_count_elem[i - 1] + offset[i - 1];
  }

  std::vector<int> local_matrix(send_count_elem[rank]);

  MPI_Scatterv(matrix.data(), send_count_elem.data(), offset.data(), MPI_INT,
               local_matrix.data(), send_count_elem[rank], MPI_INT, 0,
               MPI_COMM_WORLD);

  int local_max = MaxInMatrixSeq(local_matrix);
  int global_max = INT_MIN;

  MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

  return global_max;
}
