// Copyright 2023 Kalinin Alexandr
#include "task_1/kalinin_a_lex_strings/lex_strings.h"

int checkOrder(char* str1, char* str2) {
    if (strcmp(str1, str2) > 0)
        return 0;
    else
        return 1;
}

int parallelLexicographicStrings(char* str1, char* str2) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int ost = std::max(strlen(str1), strlen(str2)) % size;
    int n = std::max(strlen(str1), strlen(str2)) / size;
    if (rank < ost)
        n++;

    char* localStr1 = new char[n];
    char* localStr2 = new char[n];

    MPI_Scatter(str1, n, MPI_CHAR,
        localStr1, n, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Scatter(str2, n, MPI_CHAR,
        localStr2, n, MPI_CHAR, 0, MPI_COMM_WORLD);

    int local_result = checkOrder(localStr1, localStr2);
    int globalResult;
    MPI_Allreduce(&local_result, &globalResult, 1,
        MPI_C_BOOL, MPI_LAND, MPI_COMM_WORLD);

    delete[] localStr1;
    delete[] localStr2;

    return globalResult;
}
