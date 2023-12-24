// Copyright 2023 Karagodin Andrey

#include "task_1/karagodin_a_string_sentences/string_sentences.h"
#include <mpi.h>
#include <vector>

int SentenceCounter::countSentences(const std::string& input) {
    int stringSize = input.length();
    int rank = 0, size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int globalCount = 0;
    std::vector<int> receive_counts(size);
    std::vector<int> shift(size);

    // Spread string across processes
    int chunk_size = stringSize / size;
    int remainder = stringSize % size;
    for (int i = 0; i < size; i++) {
        receive_counts[i] = chunk_size;
        if (i < remainder) {
            receive_counts[i]++;
        }
        shift[i] = (i > 0) ? (shift[i - 1] + receive_counts[i - 1]) : 0;
    }

    std::string receive_data(input, shift[rank], receive_counts[rank]);
    MPI_Scatterv(input.data(), receive_counts.data(), shift.data(), MPI_CHAR,
               receive_data.data(), receive_counts[rank], MPI_INT, 0,
               MPI_COMM_WORLD);
    int localCount = countFunc(input);
    MPI_Reduce(&localCount, &globalCount, 1, MPI_INT,
     MPI_SUM, 0, MPI_COMM_WORLD);
    return globalCount;
}

int countFunc(const std::string& input) {
    int count = 0;
    for (int i = 0; i < input.length(); i++) {
    if (input[i] == '.' || input[i] == '!' || input[i] == '?') {
      count++;
    }
  }
  return count;
}
