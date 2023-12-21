// Copyright 2023 Kalinin Alexandr
#include "task_1/kalinin_a_lex_strings/lex_strings.h"

std::vector<int> getRandomMatrix(std::vector<int>::size_type row_count, std::vector<int>::size_type column_count) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(row_count * column_count);
    for (std::vector<int>::size_type i = 0; i < row_count; i++) {
        for (std::vector<int>::size_type j = 0; j < column_count; j++) {
            vec[i * column_count + j] = gen() % 100;
        }
    }
    return vec;
}

std::vector<int> getSequentialOperations(const std::vector<int>& matrix1, const std::vector<int>& matrix2,
    std::vector<int>::size_type row_count_matrix1, std::vector<int>::size_type column_count_matrix1,
    std::vector<int>::size_type column_count_matrix2) {

    std::vector<int>::size_type row_count_matrix2 = column_count_matrix1;

    std::vector<int>::size_type res_size = row_count_matrix1 * column_count_matrix2;
    std::vector<int> res(res_size, 0);

    for (std::vector<int>::size_type i = 0; i < row_count_matrix1; i++) {
        for (std::vector<int>::size_type j = 0; j < column_count_matrix2; j++) {
            for (std::vector<int>::size_type k = 0; k < column_count_matrix1; k++) {
                res[i * column_count_matrix2 + j] +=
                    matrix1[i * row_count_matrix2 + k] * matrix2[k * column_count_matrix2 + j];
            }
        }
    }
    return res;
}