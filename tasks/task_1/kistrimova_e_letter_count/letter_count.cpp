// Copyright 2023 Kistrimova Ekaterina
#include "task_1/kistrimova_e_letter_count/letter_count.h"



int count_letters_seq(const std::string& str) {
    int count = 0;
    for (char ch : str) {
        if (std::isalpha(ch)) {
            count++;
        }
    }
    return count;
}
int count_letters_par(const std::string& str) {
    int numproc;
    MPI_Comm_size(MPI_COMM_WORLD, &numproc);

    int rankproc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankproc);

    int str_Size =  str.size();

    int l_size = str_Size / numproc;
    int rest = str_Size % numproc;

    int start = rankproc * l_size + (rankproc < rest ? rankproc : rest);
    int end = start + l_size + (rankproc < rest);

    std::string local_str = str.substr(start, end - start);

    int l_count = count_letters_seq(local_str);
    int g_count = 0;

    MPI_Reduce(&l_count, &g_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return g_count;
}
std::string generateRandomRow(int size) {
    const std::string dict =
        "nhbdhvbhdbvhdhvh328438756474MSXDCFVGBH!@#$%^&*()_+№;%:?*-=,`~<>";
    std::string result;
    result.reserve(size);
    for (int i = 0; i < size; ++i) {
        result.push_back(dict[std::rand() % dict.size()]);
    }
    return result;
}
