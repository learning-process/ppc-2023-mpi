// Copyright 2024 Kokin Ivan

#include "task_1/kokin_sentence_count/count_sentences.h"
#include <mpi.h>
#include <vector>

int countFunc(const std::string &str) {
  int count = 0;
  for (int i = 0; i < str.length(); i++) {
    if (str[i] == '.' || str[i] == '!' || str[i] == '?') {
      count++;
    }
  }
  return count;
}

int countSentences(const std::string &str) {
  int size = 0, rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> receive_count(size);
  std::vector<int> shift(size);

  int stringSize = str.length();
  int sequence_size = stringSize / size;
  int chunk = stringSize % size;

  for (int i = 0; i < size; i++) {
    receive_count[i] = sequence_size;
    if (i < chunk) {
      receive_count[i]++;
    }
    shift[i] = (i > 0) ? (shift[i - 1] + receive_count[i - 1]) : 0;
  }

  std::string receive_data(str, shift[rank], receive_count[rank]);
  MPI_Scatterv(str.data(), receive_count.data(), shift.data(), MPI_CHAR,
               receive_data.data(), receive_count[rank], MPI_INT, 0,
               MPI_COMM_WORLD);

  int localCount = countFunc(receive_data);
  int globalCount = 0;

  MPI_Reduce(&localCount, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return globalCount;
}
