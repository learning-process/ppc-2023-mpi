// Copyright 2023 Tushentsova Karina
#include "task_1/tushentsova_k_count_mis_char/count_mis_char.h"

int countNonMatchingChars(
        const std::string &str1, const std::string &str2, int from, int to) {
    int count = 0;
    for (int i = from; i < to; i++) {
        if (str1[i] != str2[i])
            count++;
    }
    return count;
}

int parCountNonMatchingChars(const std::string &str1, const std::string &str2) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int minStrSize = 0;
    if (str1.size() < str2.size()) {
        minStrSize = str1.size();
    } else {
        minStrSize = str2.size();
    }

    int procStrLen = minStrSize / size;
    int remainder = minStrSize % size != 0;

    int step = procStrLen + remainder;
    int from = step * rank;
    int to = 0;
    if (minStrSize < step * (rank + 1)) {
        to = minStrSize;
    } else {
        to = step * (rank + 1);
    }

    int localCount = countNonMatchingChars(str1, str2, from, to);

    int count = 0;
    MPI_Reduce(&localCount, &count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    int str1Size = str1.size();
    int str2Size = str2.size();
    int stringSizeDiff = std::abs(str1Size - str2Size);
    int totalCount = count + stringSizeDiff;

    return totalCount;
}
