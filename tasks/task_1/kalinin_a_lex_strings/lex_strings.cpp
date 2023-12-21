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
    if (rank < ost1)
        n1++;
    if (rank < ost2)
        n2++;

    char* localStr1 = new char[n1 + 1];
    char* localStr2 = new char[n2 + 1];

    MPI_Scatter(str1, n1, MPI_CHAR,
        localStr1, n1, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Scatter(str2, n2, MPI_CHAR,
        localStr2, n2, MPI_CHAR, 0, MPI_COMM_WORLD);

    localStr1[n1] = '\0';
    localStr2[n2] = '\0';

    bool local_result = checkOrder(localStr1, localStr2);
    bool globalResult;
    MPI_Allreduce(&local_result, &globalResult, 1,
        MPI_C_BOOL, MPI_LAND, MPI_COMM_WORLD);

    delete[] localStr1;
    delete[] localStr2;

    return globalResult;
}
