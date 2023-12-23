// Copyright 2023 Derun Andrew
#include "task_3/derun_a_radix_batcher/radix_batcher.h"

#include <mpi.h>

#include <algorithm>
#include <cmath>
#include <ctime>
#include <random>
#include <utility>
#include <vector>

std::vector<double> getRandomVector(const int size) {
  std::mt19937 gen(time(0));
  std::uniform_real_distribution<> urd(0, 300);
  std::vector<double> vector(size);
  for (double& value : vector) value = urd(gen);

  return vector;
}

void oddEvenMerge(std::vector<double>* arr, int n, int lo, int r) {
  int m = r * 2;

  if (m < n) {
    oddEvenMerge(arr, n, lo, m);
    oddEvenMerge(arr, n, lo + r, m);

    for (int i = lo + r; i + r < lo + n; i += m) {
      if ((*arr)[i] > (*arr)[i + r]) std::swap((*arr)[i], (*arr)[i + r]);
    }
  } else {
    if ((*arr)[lo] > (*arr)[lo + r]) {
      std::swap((*arr)[lo], (*arr)[lo + r]);
    }
  }
}

std::vector<double> merge(std::vector<std::vector<double>> vector_array) {
  while (vector_array.size() != 1) {
    for (int i = 0; i < static_cast<int>(vector_array.size() - 1); i++) {
      auto temp = vector_array[i];
      temp.insert(temp.end(), vector_array[i + 1].begin(),
                  vector_array[i + 1].end());
      oddEvenMerge(&temp, temp.size());
      vector_array[i] = temp;
      vector_array.erase(vector_array.begin() + i + 1);
    }
  }
  return vector_array[0];
}

std::vector<double> RadixSortParallel(std::vector<double> vec, int size) {
  int nproc, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int delta = size / nproc;
  std::vector<double> local_vec, global_vec;

  local_vec.resize(delta);

  MPI_Scatter(vec.data(), delta, MPI_DOUBLE, local_vec.data(), delta,
              MPI_DOUBLE, 0, MPI_COMM_WORLD);
  double* data = local_vec.data();
  floatRadixSort<double>(&data, delta);

  if (rank != 0) {
    MPI_Send(local_vec.data(), delta, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  } else {
    std::vector<std::vector<double>> vec_array;
    vec_array.push_back(local_vec);

    for (int i = 1; i < nproc; ++i) {
      MPI_Recv(local_vec.data(), delta, MPI_DOUBLE, i, 0, MPI_COMM_WORLD,
               MPI_STATUSES_IGNORE);
      vec_array.push_back(local_vec);
    }

    global_vec = merge(vec_array);
  }

  return global_vec;
}
