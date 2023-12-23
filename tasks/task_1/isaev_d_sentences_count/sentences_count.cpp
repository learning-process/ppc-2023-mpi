// Copyright 2023 Isaev Dmitriy

#include "task_1/isaev_d_sentences_count/sentences_count.h"


int countSequence(const std::string &str) {
  int count = 0;
  for (int i = 0; i < str.length(); i++) {
    if (str[i] == '.' || str[i] == '!' || str[i] == '?') {
      count++;
    }
  }
  return count;
}

int countParallel(const std::string &str) {
  int numProc;
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  int rankProc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

  int strSize = str.length();

  std::vector<int> recv_counts(numProc);
  std::vector<int> displs(numProc);

  int chunk_size = strSize / numProc;
  int remainder = strSize % numProc;

  for (int i = 0; i < numProc; i++) {
    recv_counts[i] = chunk_size;
    if (i < remainder) {
      recv_counts[i]++;
    }
    displs[i] = (i > 0) ? (displs[i - 1] + recv_counts[i - 1]) : 0;
  }

  // string(string & S, size_t start, size_t len)

  std::string recv_data(str, displs[rankProc], recv_counts[rankProc]);
  MPI_Scatterv(str.data(), recv_counts.data(), displs.data(), MPI_CHAR,
               recv_data.data(), recv_counts[rankProc], MPI_INT, 0,
               MPI_COMM_WORLD);

  int localCount = countSequence(recv_data);
  int globalCount = 0;

  MPI_Reduce(&localCount, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return globalCount;
}
