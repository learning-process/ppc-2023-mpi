// Copyright 2023 Chuvashov Andrey

#include "../tasks/task_1/chuvashov_a_count_num_of_char_in_string/count_num_of_char_in_string.h"

std::string GenerateRandomString(size_t strSize) {
    std::string str;
    std::srand(static_cast<int>(time(0)));
    for (size_t i = 0; i < strSize; i++) {
        str += static_cast<char>(std::rand() % (122 - 33 + 1) + 33);
    }
    return str;
}

std::string GenerateRandomOnlyLettersString(size_t strSize) {
    static const char alphabet[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string str;
    std::srand(static_cast<int>(time(0)));
    for (size_t i = 0; i < strSize; i++) {
        str += alphabet[std::rand() % 52];
    }
    return str;
}

int ParallelLettersCount(const std::string& str) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int remains = str.size() % size, count = 0;
    int fragmentLength = (str.size() / size) + 1 * (rank < remains);

    if (rank == 0) {
        int fragSize, start;
        for (int proc = 1; proc < size; proc++) {
            fragSize = (str.size() / size) + 1 * (proc < remains);

            start = (proc < remains ?
                fragSize * proc :
                (fragSize + 1) * remains + (proc - remains) * fragSize);

            MPI_Send(str.substr(start, str.size() - fragSize).data(),
                fragSize, MPI_CHAR, proc, 0, MPI_COMM_WORLD);
        }
    }

    std::string fragment;
    if (rank == 0) {
        fragment = std::string(str.begin(), str.begin() + fragmentLength);
    } else {
        MPI_Status status;
        MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_CHAR, &fragmentLength);
        fragment.resize(fragmentLength);
        MPI_Recv(&(fragment[0]), fragmentLength,
            MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    }

    int fragmentCount = SequentialLettersCount(fragment);
    MPI_Reduce(&fragmentCount, &count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return count;
}

int SequentialLettersCount(const std::string& str) {
    int counter = 0;
    for (char letter : str) {
        if (std::isalpha(letter)) {
            counter++;
        }
    }
    return counter;
}
