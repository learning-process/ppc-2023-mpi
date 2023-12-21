// Copyright 2023 Kalinin Alexandr
#include "task_2/kalinin_a_gorA_verB/gorA_verB.h"

std::vector<int> getRandomMatrix(int row_count,
    int column_count) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(row_count * column_count);
    for (int i = 0; i < row_count * column_count; i++) {
        vec[i] = gen() % 100;
    }
    return vec;
}

std::vector<int> getSequentialOperations(const
    std::vector<int>& matrix1, const std::vector<int>& matrix2,
    int row_count_matrix1, int column_count_matrix1,
    int column_count_matrix2) {

    std::vector<int> res(row_count_matrix1 *
        column_count_matrix2, 0);

    for (int i = 0; i < row_count_matrix1; i++) {
        for (int j = 0; j < column_count_matrix2; j++) {
            for (int k = 0; k < column_count_matrix1; k++) {
                res[i * column_count_matrix2 + j] +=
                    matrix1[i * column_count_matrix1 + k]
                    * matrix2[k * column_count_matrix2 + j];
            }
        }
    }
    return res;
}
