// Copyright 2023 Kalinin Alexandr
#include "task_1/kalinin_a_lex_strings/lex_strings.h"

bool checkOrder(const std::string& str1, const std::string& str2) {
    return str1 <= str2;
}

bool parallelLexicographicStrings(const std::string& str1,
    const std::string& str2) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int ost = std::max(str1.size(), str2.size()) % size;
    int n = std::max(str1.size(), str2.size()) / size;
    if (rank < ost)
        n++;

    std::vector<char> localStr1(n);
    std::vector<char> localStr2(n);

    MPI_Scatter(str1.data(), n, MPI_CHAR,
        localStr1.data(), n, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Scatter(str2.data(), n, MPI_CHAR,
        localStr2.data(), n, MPI_CHAR, 0, MPI_COMM_WORLD);

    bool localResult = checkOrder(std::string(localStr1.begin(),
        localStr1.end()), std::string(localStr2.begin(), localStr2.end()));
    bool globalResult;
    MPI_Allreduce(&localResult, &globalResult, 1,
        MPI_C_BOOL, MPI_LAND, MPI_COMM_WORLD);

    return globalResult;
}
