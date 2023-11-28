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
    int sizeWorld = 0;
    int rank = 0;
    int localCount = 0, globalCount = 0;
    size_t size = str.size();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);

    int basicSize = size / sizeWorld;
    int remainder = size % sizeWorld;

    std::vector<int> stringSize(sizeWorld);
    std::vector<int> displacement(sizeWorld);

    int d = 0;
    for (int i = 0; i < sizeWorld; ++i) {
        stringSize[i] = (i < remainder) ? basicSize + 1 : basicSize;
        displacement[i] = d;
        d += stringSize[i];
    }


    std::string localString = str.substr(displacement[rank], stringSize[rank]);

    localCount = countSentences(localString);
    MPI_Reduce(&localCount, &globalCount,
        1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return globalCount;
}
