// Copyright 2023 Sokolova Daria

#include "./counting_the_number_of_sentences_in_string.h"
#include <mpi.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>



int countSentences(std::string line) {
    int count = 0;
    for (char c : line) {
        if (c == '.' || c == '!' || c == '?'
            || c == '?!' || c == '...'
            || c == '?..' || c == '!!!') {
            count++;
        }
    }
    return count;
}

int parallelCountSentencesInString(const std::string& str) {
    int rank, size, local_count = 0, global_count = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_size = str.size() % size == 0 ?
        str.size() / size :
        str.size() / size + 1;
    int start = rank * local_size;

    std::vector<int> recv_counts(size);
    std::vector<int> displs(size);
    for (int i = 0; i < size; ++i) {
        recv_counts[i] = (i == size - 1)
            ? str.size() - (i * local_size)
            : local_size;
        displs[i] = i * local_size;
    }

    std::string recv_data;
    recv_data.resize(recv_counts[rank]);
    MPI_Scatterv(str.data(), recv_counts.data(),
        displs.data(), MPI_CHAR, &recv_data[0],
        recv_counts[rank], MPI_CHAR, 0, MPI_COMM_WORLD);
    int localSum = countSentences(recv_data);
    int globalSum = 0;

    MPI_Reduce(&localSum, &globalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);


    return globalSum;
}
