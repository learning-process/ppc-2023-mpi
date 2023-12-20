// Copyright 2023 Nesterov Alexander
#include <vector>
#include <cmath>
#include <limits>
#include <string>
#include <random>
#include <iostream>
#include <algorithm>
#include <functional>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "examples/test_mpi/ops_mpi.h"
#include "../tasks/task_1/Kazantsev_e_near_neighbor_el/near_neighbor_el.h"

std::vector<int> getRandVec(int size) {
  std::vector<int> vec(size);
  std::random_device d;
  std::mt19937 gen(d());
  for (int i = 0; i < size; ++i) {
    vec[i] = gen() % 100;
  }
  return vec;
}
int SequentialCount(const std::vector<int>& vec) {
  if (vec.size() == 0 || vec.size() == 1) {
    return 0;
  }
  int lenght = vec.size();
  int min = abs(vec[1] - vec[0]);
  for (int i = 2; i < lenght; ++i) {
    if (min > abs(vec[i] - vec[i - 1])) {
      min = abs(vec[i] - vec[i - 1]);
    }
  }
  return min;
}
int parallelCount(const std::vector<int>& vec, const int sz) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = sz / size;
  int ost = sz % size;
  std::vector<int> local_vec;
  if (rank == 0) {
    if (n > 0) {
      for (int proc = 1; proc < size; proc++) {
        MPI_Send(vec.data() + ost + proc * n - 1, n + 1,
        MPI_INT, proc, 0, MPI_COMM_WORLD);
      }
    }
  }
  if (rank == 0) {
    local_vec = std::vector<int>(vec.begin(), vec.begin() + n + ost);
  } else {
    if (n > 0) {
      local_vec.resize(n + 1);
      MPI_Status status;
      MPI_Recv(local_vec.data(), n + 1,
      MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
  }
  int my_p = INT16_MAX;
  if (local_vec.size() > 1) {
    my_p = SequentialCount(local_vec);
  }
  int global_p;
  MPI_Reduce(&my_p, &global_p, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
  return global_p;
}
