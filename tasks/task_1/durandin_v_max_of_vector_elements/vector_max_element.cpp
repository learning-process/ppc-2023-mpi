// Copyright 2023 Durandin Vladimir
#include "task_1/durandin_v_max_of_vector_elements/vector_max_element.h"

int64_t sub_get_max_element_of_vector(const std::vector<int64_t>& _vector) {
  if (_vector.empty()) return std::numeric_limits<int64_t>::min();
  int64_t max_value = _vector.front();
  for (auto it = _vector.cbegin() + 1ull; it != _vector.cend(); ++it)
    if (*it > max_value) max_value = *it;

  return max_value;
}

int64_t par_get_max_element_of_vector(const std::vector<int64_t>& _vector) {
  int rank, proc_count;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

  const int sendcount = static_cast<int>(ceil(
      static_cast<double>(_vector.size()) / static_cast<double>(proc_count)));
  std::vector<int> sendcounts(proc_count, sendcount);
  sendcounts[static_cast<size_t>(proc_count - 1)] =
      static_cast<int>(_vector.size()) - (proc_count - 1) * sendcount;

  std::vector<int> displs(proc_count);
  displs[0ull] = 0;
  for (size_t i = 1ull; i < proc_count; ++i)
    displs[i] = displs[i - 1ull] + sendcount;

  std::vector<int64_t> recvbuf(sendcounts[rank]);
  MPI_Scatterv(_vector.data(), sendcounts.data(), displs.data(), MPI_INT64_T,
               recvbuf.data(), recvbuf.size(), MPI_INT64_T, 0, MPI_COMM_WORLD);

  const int64_t local_max = sub_get_max_element_of_vector(recvbuf);

  int64_t global_max;
  MPI_Reduce(&local_max, &global_max, 1, MPI_INT64_T, MPI_MAX, 0,
             MPI_COMM_WORLD);

  return global_max;
}

std::vector<int64_t> create_random_vector(const size_t size, const int64_t min,
                                          const int64_t max) {
  if (size == 0ull) return std::vector<int64_t>();
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_int_distribution<int64_t> distance(min, max);

  std::vector<int64_t> vector(size);
  for (size_t i = 0ull; i < size; ++i) vector[i] = distance(generator);
  return vector;
}
