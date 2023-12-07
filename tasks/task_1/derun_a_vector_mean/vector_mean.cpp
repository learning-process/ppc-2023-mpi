// Copyright 2023 Derun Andrew

#include "task_1/derun_a_vector_mean/vector_mean.h"

double meanSeq(const std::vector<int> &vec) {
  double total = 0.0;
  for (int i = 0; i < vec.size(); i++) {
    total += vec[i];
  }

  return total / static_cast<double>(vec.size());
}

double meanPar(const std::vector<int> &vec) {
  int numProc;
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  int rankProc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

  int vecSize = vec.size();

  std::vector<int> recv_counts(numProc);
  std::vector<int> displs(numProc);

  int chunk_size = vecSize / numProc;
  int remainder = vecSize % numProc;

  for (int i = 0; i < numProc; i++) {
    recv_counts[i] = chunk_size;
    if (i < remainder) {
      recv_counts[i]++;
    }
    displs[i] = (i > 0) ? (displs[i - 1] + recv_counts[i - 1]) : 0;
  }

  std::vector<int> recv_data(recv_counts[rankProc]);
  MPI_Scatterv(vec.data(), recv_counts.data(), displs.data(), MPI_INT,
               recv_data.data(), recv_counts[rankProc], MPI_INT, 0,
               MPI_COMM_WORLD);

  double localSum = meanSeq(recv_data);
  double globalSum = 0.0;

  MPI_Reduce(&localSum, &globalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  return globalSum / static_cast<double>(numProc);
}

int generateRandomNumber(int min, int max) {
  int randomNumber = min + (std::rand() % (max - min + 1));

  return randomNumber;
}
