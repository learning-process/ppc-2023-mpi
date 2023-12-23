// Copyright 2023 Soloninko Andrey

#include "task_1/soloninko_a_non_matching_symbols/Algo.h"
#include <vector>

std::string random_string(int length) {
    const std::string CHARACTERS = "abcdefghijklmnopqrstuvwxyz";
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);
    std::string random_string;

    for (int i = 0; i < length; ++i) {
        random_string += CHARACTERS[distribution(generator)];
    }

    return random_string;
}

int GetStringDifDef(const std::string &s1, const std::string &s2) {
    int dif_coint = 0;
    for (int i = 0; i < s1.size(); i++) {
        if (s1[i] != s2[i]) {
            dif_coint++;
        }
    }
    return dif_coint;
}

int GetStringDifPar(const std::string &s1, const std::string &s2) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int str_len = s1.length();
    std::vector<int> sendcounts(size);
    std::vector<int> displs(size);

    int segment_size = str_len / size;
    int remainder = str_len % size;

    for (int i = 0; i < size; i++) {
        sendcounts[i] = segment_size;
        if (i < remainder) {
            sendcounts[i]++;
        }

        if (i == 0) {
            displs[i] = 0;
        } else {
            displs[i] = displs[i - 1] + sendcounts[i - 1];
        }
    }

    std::string local_str1(sendcounts[rank], ' ');
    std::string local_str2(sendcounts[rank], ' ');

    MPI_Scatterv(s1.c_str(), sendcounts.data(), displs.data(), MPI_CHAR,
                 &local_str1[0], sendcounts[rank], MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Scatterv(s2.c_str(), sendcounts.data(), displs.data(), MPI_CHAR,
                 &local_str2[0], sendcounts[rank], MPI_CHAR, 0, MPI_COMM_WORLD);

    int local_dif_count = 0;

    for (int i = 0; i < local_str1.length(); i++) {
        if (local_str1[i] != local_str2[i]) {
            local_dif_count++;
        }
    }

    int dif_count = 0;
    MPI_Reduce(&local_dif_count, &dif_count, 1, MPI_INT, MPI_SUM, 0,
               MPI_COMM_WORLD);

    return dif_count;
}
