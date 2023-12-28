// Copyright 2023 Nikitin Kirill

#include "task_1/nikitin_k_word_count/count_words.h"

int getRandInt(int lBorder, int rBorder) {
    return rand() % (rBorder - lBorder + 1) + lBorder;
}

std::string getRandString() {
    srand(time(nullptr));
    std::string res = " ", alphabet =
    "q w e r t y u i o p a s d f g h j k l z x c v b n m 1 2 3 4 5 6 7 8 9 0";
    int strSize = getRandInt(50, 1000);
    for (int i = 0; i < strSize; ++i) {
        res += alphabet[getRandInt(0, alphabet.size() - 1)];
    }
    return res;
}

int sequentialCountWordsInString(const std::string& str) {
    std::stringstream stream(str);
    std::istream_iterator<std::string> start(stream), end;
    return std::distance(start, end);
}

int countWords(std::string strLine, bool isWord) {
    int wCount = 0;
    for (char c : strLine) {
        if (c == ' ' || c == '\t' || c == '\n') {
            isWord = false;
        } else if (!isWord) {
            isWord = true;
            wCount++;
        }
    }

    return wCount;
}

int parallelCountWordsInString(const std::string& str) {
    int rank, size, local_count = 0, global_count = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_size = str.size() % size == 0 ?
        str.size() / size : str.size() / size + 1;

    int start = rank * local_size;
    if (start < str.size()) {
        bool isWord;
        if (rank == 0 || str[start - 1] == ' '
            || str[start - 1] == '\t' || str[start - 1] == '\n')
            isWord = false;
        else
            isWord = true;
        if (start + local_size >= str.size()) {
            local_size = str.size() - start;
        }
        std::string local_str = str.substr(start, local_size);

        local_count = countWords(local_str, isWord);
    } else {
        local_count = 0;
    }

    MPI_Reduce(&local_count, &global_count, 1,
        MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return global_count;
}
