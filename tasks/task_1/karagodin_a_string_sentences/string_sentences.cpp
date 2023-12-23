// Copyright 2023 Karagodin Andrey

#include "task_1/karagodin_a_string_sentences/string_sentences.h"
#include <mpi.h>

int SentenceCounter::countSentences(const std::string& input) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int localCount = 0;
    int globalCount;

    // Spread string across processes
    int localStart = rank * (input.size() / size);
    int localEnd = (rank + 1) * (input.size() / size);
    if (rank == size - 1) {
        localEnd = input.size();
    }
    std::string localInput = input.substr(localStart, localEnd - localStart);

    // Counting sentences amount
    for (int i = 0; i < localInput.size(); i++) {
        if (localInput[i] == '.' || localInput[i] == '!'
         || localInput[i] == '?') {
            localCount++;
        }
    }

    // Gather results from all processes
    MPI_Reduce(&localCount, &globalCount, 1,
     MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return globalCount;
}
