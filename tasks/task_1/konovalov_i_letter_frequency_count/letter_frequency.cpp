// Copyright 2023 Konovalov Igor
#include "task_1/konovalov_i_letter_frequency_count/letter_frequency.h"


double letterFreqCalcSeq(const char* str, char l) {
  int count = 0;
  int i;
  for (i = 0; str[i] != '\0'; i++) {
	if (str[i] == l) {
	  count++;
	}
  }
  return static_cast<double>(count) / static_cast<double>(i);
}

int letterCountInChunk(const char* str, char l) {
  int count = 0;
  for (int i = 0; str[i] != '\0'; i++) {
	if (str[i] == l) {
	  count++;
	}
  }
  return count;
}

double letterFreqCalcPar(const char* str, char l) {
  int len;
  for (len = 0; str[len] != '\0'; len++) {}

  int numProc;
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  int rankProc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

  std::vector<int> recv_counts(numProc);
  std::vector<int> displs(numProc);

  for (int i = 0; i < numProc; i++) {
	recv_counts[i] = 0;
  }

  int chunk_size = len / numProc;
  int remaining = len % numProc;

  for (int i = 0; i < numProc; i++) {
	recv_counts[i] = chunk_size;
	if (i < remaining) {
	  recv_counts[i]++;
	}
	displs[i] = (i > 0) ? (displs[i - 1] + recv_counts[i - 1]) : 0;
  }

  std::string recv_data(str, displs[rankProc], recv_counts[rankProc]);
  MPI_Scatterv(str, recv_counts.data(), displs.data(), MPI_CHAR, recv_data.data(),
	recv_counts[rankProc], MPI_INT, 0, MPI_COMM_WORLD);

  int localCount = letterCountInChunk(recv_data.data(), l);
  int globalCount = 0;

  MPI_Reduce(&localCount, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return static_cast<double>(globalCount) / static_cast<double>(len);
}
