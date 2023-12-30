// Copyright 2023 Akopyan Zal

#include "task_1/akopyan_z_symbols_count/symbols_count.h"

int getRandInt(int leftBorder, int rightBorder) {
    return rand() % (rightBorder - leftBorder + 1) + leftBorder;
}

std::string getRandString() {
    srand(time(nullptr));
    std::string res = " ", alphabit = "qwertyuiopasdfghjklzxcvbnm1234567890 ";
    int strSize = getRandInt(50, 1000);
    for (int i = 0; i < strSize; ++i) {
        res += alphabit[getRandInt(0, alphabit.size() - 1)];
    }
    return res;
}

int sequentialCountSymbolsInString(const std::string& str) {
    int count = 0;
    for (char c : str) {
        if (isalpha(c))
            count++;
    }
    return count;
}

int countSymbols(std::string line) {
    int count = 0;
    for (char c : line) {
        if (isalpha(c))
            count++;
    }
    return count;
}

int parallelCountSymbolsInString(const std::string& str) {
    int rank, size, local_count = 0, global_count = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_size = str.size() % size == 0 ? str.size() / size : str.size() / size + 1;

    int start = rank * local_size;
    if (start < str.size()) {
        if (start + local_size >= str.size()) {
            local_size = str.size() - start;
        }
        std::string local_str = str.substr(start, local_size);

        local_count = countSymbols(local_str);
    }

    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return global_count;
}
