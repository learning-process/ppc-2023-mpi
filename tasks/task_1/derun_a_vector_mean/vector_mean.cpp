// Copyright 2023 Derun Andrew
#include "task_1/derun_a_vector_mean/vector_mean.h"

#include <mpi.h>

#include <algorithm>
#include <random>
#include <vector>

std::vector<int> getRandomVector(int size) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(1, 100);

  if (size < 0) {
    size = 0;
  }
  std::vector<int> vec(size);
  for (int i = 0; i < size; i++) {
    vec[i] = dist(gen);
  }
  return vec;
}

int getParallelMean(std::vector<int> parall_vec, int size) {
  int ProcRank, ProcNum, sum_all;
  float Paverage;

  if (size <= 0) {
    return 0;
  }

  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int partSize = size / ProcNum;
  if (ProcRank == 0) {
    for (int i = 1; i < ProcNum; i++) {
      MPI_Send(parall_vec.data() + i * partSize, partSize, MPI_INT, i, 0,
               MPI_COMM_WORLD);
    }
  }

  std::vector<int> local_vec(partSize);
  if (ProcRank == 0) {
    local_vec =
        std::vector<int>(parall_vec.begin(), parall_vec.begin() + partSize);
  } else {
    MPI_Status status;
    MPI_Recv(local_vec.data(), partSize, MPI_INT, 0, 0, MPI_COMM_WORLD,
             &status);
  }

  int sum = sum_all = 0;
  for (int i = 0; i < partSize; i++) {
    sum += local_vec[i];
  }

  MPI_Reduce(&sum, &sum_all, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  Paverage = static_cast<float>(sum_all) / size;
  return Paverage;
}

int getSequentialMean(std::vector<int> sequent_vec) {
  int size = sequent_vec.size(), sum_all = 0;
  float Saverage = 0;

  if (size <= 0) {
    return 0;
  }

  for (int i = 0; i < size; i++) {
    sum_all += sequent_vec[i];
  }

  Saverage = static_cast<float>(sum_all) / size;
  return Saverage;
}
