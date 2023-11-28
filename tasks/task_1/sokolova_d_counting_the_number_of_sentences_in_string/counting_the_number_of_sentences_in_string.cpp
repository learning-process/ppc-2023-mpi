// Copyright 2023 Sokolova Daria

#include "./counting_the_number_of_sentences_in_string.h"
#include <mpi.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>



std::string getRandomSentence() {
    srand(static_cast<unsigned int>(time(nullptr)));

    const std::string characters =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!? ";
    int sentenceLength = std::rand() % 100;
    std::string randomSentence;
    const std::string punctuations = ".!?";
    for (int i = 0; i < sentenceLength; ++i) {
        int charIndex = std::rand() % characters.size();
        randomSentence += characters[charIndex];
    }

    randomSentence[0] = std::toupper(randomSentence[0]);
    int punctuationIndex = std::rand() % punctuations.size();
    randomSentence += punctuations[punctuationIndex];

    return randomSentence;
}
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
    /*int rank, size, local_count = 0, global_count = 0;

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

    return globalSum;*/
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
