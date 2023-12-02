// Copyright 2023 Safarov Nurlan
#include "task_1/safarov_n_non_matching_symbols/number_of_different_symbols.h"

int numberOfNonMatchingCharacters_Consistently(const std::string& firstString,
    const std::string& secondString) {
    int mn = std::min(firstString.length(), secondString.length());
    int mx = std::max(firstString.length(), secondString.length());
    int difference_count = mx - mn;

    for (int i = 0; i < mn; ++i) {
        if (firstString[i] != secondString[i]) {
            difference_count++;
        }
    }

    return difference_count;
}

int numberOfNonMatchingCharacters_Parallel(std::string* firstString,
    std::string* secondString) {
    int size, rank, globalCountResult = 0, localCountResult = 0, l;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    if ((*firstString).length() > (*secondString).length()) {
        for (int j = (*secondString).length(); j < (*firstString).length() - (*secondString).length(); ++j) {
            (*secondString) += "$";
        }
        l = (*firstString).length();
    } else if ((*firstString).length() < (*secondString).length()) {
        for (int j = (*firstString).length(); j < (*secondString).length() - (*firstString).length(); ++j) {
            (*firstString) += "$";
        }
        l = (*secondString).length();
    } else {
        l = (*firstString).length();
    }

    int* n = new int[size];
    int* offsets = new int[size];

    int remainder = l % size;
    int offset = 0;
    for (int i = 0; i < size; ++i) {
        n[i] = (l / size) + (i < remainder ? 1 : 0);
        offsets[i] = offset;
        offset += n[i];
    }

    char* local_first_string = new char[n[rank] + 1];
    char* local_second_string = new char[n[rank] + 1];

    MPI_Scatterv((*firstString).c_str(), n, offsets, MPI_CHAR,
        local_first_string, n[rank], MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Scatterv((*secondString).c_str(), n, offsets, MPI_CHAR,
        local_second_string, n[rank], MPI_CHAR, 0, MPI_COMM_WORLD);

    local_first_string[n[rank]] = '\0';
    local_second_string[n[rank]] = '\0';

    for (int i = 0; i < n[rank]; ++i) {
        if (local_first_string[i] != local_second_string[i]) {
            localCountResult++;
        }
    }

    MPI_Reduce(&localCountResult, &globalCountResult, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    delete[] n;
    delete[] offsets;
    delete[] local_first_string;
    delete[] local_second_string;

    return globalCountResult;
}

std::string generateRandomString(const int& length) {
    const std::string CHARACTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> distr(0, CHARACTERS.length() - 1);

    std::string randomString;
    for (int i = 0; i < length; ++i) {
        randomString += CHARACTERS[distr(gen)];
    }
    return randomString;
}
