// Copyright 2023 Tushentsova Karina
#include "task_1/tushentsova_k_count_mis_char/count_mis_char.h"

int countNonMatchingChars(const std::string &str1, const std::string &str2) {
    int count = 0, length = 0;
    if (str1.length() < str2.length()) {
        length = str1.length();
    } else {
        length = str2.length();
    }
    for (int i = 0; i < length; i++) {
        if (str1[i] != str2[i])
            count++;
    }
    return count;
}

int parCountNonMatchingChars(const std::string &str1, const std::string &str2) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int lCount = 0;
    int procStrLen = str1.length() / size;
    int remainder = str1.length() % size;

    std::string localStr1, localStr2;
    if (rank < remainder) {
        localStr1 = str1.substr(rank * (procStrLen + 1), procStrLen + 1);
        localStr2 = str2.substr(rank * (procStrLen + 1), procStrLen + 1);
    } else {
        localStr1 = str1.substr(rank * procStrLen + remainder, procStrLen);
        localStr2 = str2.substr(rank * procStrLen + remainder, procStrLen);
    }

    lCount = countNonMatchingChars(localStr1, localStr2);

    int *recvCounts = new int[size];
    MPI_Allgather(&lCount, 1, MPI_INT, recvCounts, 1, MPI_INT, MPI_COMM_WORLD);

    int totalCount = 0;
    for (int i = 0; i < size; i++) {
        totalCount += recvCounts[i];
    }

    delete[] recvCounts;

    return totalCount;
}
