// Copyright 2023 Kanakov Roman
#include "task_1/kanakov_num_of_alternations_signs/num_of_alternations_signs.h"

int seq_num_of_alternations_signs(const std::vector<int>& vec) {
  if (vec.size() < 2)
    return 0;
  size_t size = vec.size();
  int count_sign = 0;

  for (size_t i = 1; i < size; ++i)
    if (vec[i] > 0 && vec[i - 1] < 0 || vec[i] < 0 && vec[i - 1] > 0)
      ++count_sign;
  return count_sign;
}

int par_num_of_alternations_signs(const std::vector<int>& vec) {
  if (vec.size() < 2)
    return 0;

  int rank = 0;
  int world_size = 0;
  size_t size = vec.size();
  int worked = 0;
  int local_count_sign = 0;
  int global_count_sign = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int delta = size / world_size;
  int remain = size % world_size;

  std::vector<int> count_send(world_size, delta);
  std::vector<int> displs(world_size);

  for (int i = 0; i < world_size; ++i) {
    if (i < remain)
      ++count_send[i];
    displs[i] = i == 0 ? 0 : displs[i - 1] + count_send[i - 1];
    if (count_send[i])
      ++worked;
  }

  for (int i = 0; i < worked; ++i)
    ++count_send[i];
  if (count_send[worked - 1] + displs[worked - 1] > size)
    count_send[worked - 1] = size - displs[worked - 1];

  MPI_Barrier(MPI_COMM_WORLD);

  std::vector<int> localvec(count_send[rank]);
  MPI_Scatterv(vec.data(), count_send.data(), displs.data(), MPI_INT,
               localvec.data(), count_send[rank], MPI_INT, 0, MPI_COMM_WORLD);

  local_count_sign = seq_num_of_alternations_signs(localvec);

  MPI_Reduce(&local_count_sign, &global_count_sign, 1, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);

  return global_count_sign;
}
