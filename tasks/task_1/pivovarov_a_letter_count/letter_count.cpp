// Copyright 2023 Pivovarov Alexey
#include "task_1/pivovarov_a_letter_count/letter_count.h"



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
    int numProc;
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    int rankProc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

    int strSize =  str.size();

    int local_size = strSize / numProc;
    int remainder = strSize % numProc;

    int start = rankProc * local_size + (rankProc < remainder ? rankProc : remainder);
    int end = start + local_size + (rankProc < remainder);

    std::string local_str = str.substr(start, end - start);

    int local_count = count_letters_seq(local_str);
    int global_count = 0;

    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return global_count;
}
std::string generateRandomRow(int size) {
    const std::string dict = "abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+№;%:?*-=,`~<>";
    std::string result;
    result.reserve(size);
    for (int i = 0; i < size; ++i) {
        result.push_back(dict[std::rand() % dict.size()]);
    }
    return result;
}
