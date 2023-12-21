// Copyright 2023 Kalinin Alexandr
#include "task_1/kalinin_a_lex_strings/lex_strings.h"

bool checkOrder(char* str1, char* str2) {
    if (strcmp(str1, str2) > 0)
        return false;
    else
        return true;
}

bool parallelLexicographicStrings(char* str1, char* str2) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int ost1 = len1 % size;
    int ost2 = len2 % size;
    int n1 = len1 / size;
    int n2 = len2 / size;

    std::vector<int> counts(size, n1);
    std::vector<int> displs(size, 0);
    for (int i = 0; i < ost1; ++i) {
        ++counts[i];
    }
    for (int i = 1; i < size; ++i) {
        displs[i] = displs[i - 1] + counts[i - 1];
    }

    char* localStr1 = new char[counts[rank] + 1];

    MPI_Scatterv(str1, counts.data(), displs.data(), MPI_CHAR,
        localStr1, counts[rank], MPI_CHAR, 0, MPI_COMM_WORLD);

    std::fill(counts.begin(), counts.end(), n2);
    for (int i = 0; i < ost2; ++i) {
        ++counts[i];
    }
    for (int i = 1; i < size; ++i) {
        displs[i] = displs[i - 1] + counts[i - 1];
    }

    char* localStr2 = new char[counts[rank] + 1];

    MPI_Scatterv(str2, counts.data(), displs.data(), MPI_CHAR,
        localStr2, counts[rank], MPI_CHAR, 0, MPI_COMM_WORLD);

    localStr1[counts[rank]] = '\0';
    localStr2[counts[rank]] = '\0';

    bool local_result = checkOrder(localStr1, localStr2);
    bool globalResult;
    MPI_Allreduce(&local_result, &globalResult, 1,
        MPI_C_BOOL, MPI_LAND, MPI_COMM_WORLD);

    delete[] localStr1;
    delete[] localStr2;

    return globalResult;
}