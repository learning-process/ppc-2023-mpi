// Copyright 2023 Kuznetsov Artem
#include "task_1/kuznetsov_a_most_different_values/most_different.h"

std::vector<int> create_random_array(size_t size, int low, int up) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(low, up);

  std::vector<int> arr(size);
  for (auto& elem : arr) elem = dist(gen);

  return arr;
}

int seq_find_most_different(const std::vector<int>& arr) {
  size_t size = arr.size();
  int max_diff = -1;

  if (size >= 2) {
    int abs_diff = 0;
    for (size_t i = 0; i < size - 1; ++i) {
      abs_diff = abs(arr[i] - arr[i + 1]);
      if (abs_diff > max_diff) max_diff = abs_diff;
    }
  }
  return max_diff;
}

int par_find_most_different(const std::vector<int>& arr) {
  int size_world = 0;
  int rank = 0;
  size_t size_arr = arr.size();

  if (size_arr < 2) return -1;

  MPI_Comm_size(MPI_COMM_WORLD, &size_world);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int chunk = static_cast<int>(size_arr / size_world);
  const int tail = static_cast<int>(size_arr % size_world);

  std::vector<int> send_counts(size_world);
  std::vector<int> displs(size_world);

  for (int i = 0; i < size_world - 1; ++i) {
    send_counts[i] = chunk + 1;
    displs[i] = i * chunk;
  }

  send_counts[size_world - 1] = static_cast<int>(
      (tail == 0) ? chunk : size_arr - (size_world - 1) * chunk);
  displs[size_world - 1] = (size_world - 1) * chunk;

  int global_max_diff = 0;
  std::vector<int> local_arr(send_counts[rank]);

  MPI_Scatterv(arr.data(), send_counts.data(), displs.data(), MPI_INT,
               local_arr.data(), send_counts[rank], MPI_INT, 0, MPI_COMM_WORLD);

  int local_max_diff = seq_find_most_different(local_arr);

  MPI_Reduce(&local_max_diff, &global_max_diff, 1, MPI_INT, MPI_MAX, 0,
             MPI_COMM_WORLD);

  return global_max_diff;
}
