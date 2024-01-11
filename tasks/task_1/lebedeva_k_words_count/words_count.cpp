// Copyright 2023 Lebedeva Ksenia
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <functional>
#include "task_1/lebedeva_k_words_count/words_count.h"

std::string getRandString(int size) {
    char ar[60] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz       ";
    std::random_device dev;
    std::mt19937 gen(dev());
    std::string str;
    for (int i = 0; i < size; i++) {
        str.push_back(ar[gen() % 59]);
    }
    return str;
}

int getWordsCountParallel(const std::string& str, int size) {
    if (size == 0) {
        return 0;
    }

    int proc_count, current_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &current_rank);
    int delta = size / proc_count;
    if (size % proc_count) {
        delta++;
    }

    const char* start = str.c_str();
    std::string local;
    int glob_count = 0;
    int loc_count = 0;

    if (current_rank == 0) {
        for (int proc = 1; proc < proc_count; proc++) {
            int csize = std::min(delta + 1, size - delta * proc);
            if (csize > 0) {
                MPI_Send(start + delta * proc, csize,
                    MPI_CHAR, proc, 0, MPI_COMM_WORLD);
            }
        }
        int sz = std::min(delta + 1, size);
        local = std::string(start, sz);
        loc_count = getWordsCountSequentially(local, sz);
    } else {
        int csize = std::min(delta + 1, size - delta * current_rank);
        if (csize > 0) {
            char* buf = new char[csize];
            MPI_Status status;
            MPI_Recv(buf, csize, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
            local = std::string(buf, csize);
            delete[] buf;
            loc_count = getWordsCountFragment(local, csize);
        }
    }

    MPI_Reduce(&loc_count, &glob_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return glob_count;
}

int getWordsCountFragment(const std::string& str, int size) {
    int count = 0;
    for (int i = 0; i < size - 1; i++) {
        if ((str[i] == ' ') && (str[i + 1] != ' ')) {
            count++;
        }
    }

    return count;
}

int getWordsCountSequentially(const std::string& str, int size) {
    int count = 0;
    for (int i = 0; i < size - 1; i++) {
        if ((str[i] == ' ') && (str[i + 1] != ' ')) {
            count++;
        }
    }

    if (str[0] != ' ') {
        count++;
    }
    return count;
}
