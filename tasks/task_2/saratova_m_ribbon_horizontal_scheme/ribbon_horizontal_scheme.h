// Copyright 2023 Saratova Marina
#ifndef TASKS_TASK_2_SARATOVA_M_RIBBON_HORIZONTAL_SCHEME_RIBBON_HORIZONTAL_SCHEME_H_
#define TASKS_TASK_2_SARATOVA_M_RIBBON_HORIZONTAL_SCHEME_RIBBON_HORIZONTAL_SCHEME_H_

#include <mpi.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>

std::vector<std::vector<int>> generate_random_matrix(int rows, int cols,
                                         int lower_limit, int upper_limit);
bool compare_matrix(const std::vector<std::vector<int>>& matrix1,
                                const std::vector<std::vector<int>>& matrix2);
std::vector<std::vector<int>> vector_to_matrix(const std::vector<int>& vector,
                                                            int rows, int cols);
std::vector<int> matrix_to_vector(const std::vector<std::vector<int>>& matrix);
std::vector<std::vector<int>> seq_matrix_multiply
                            (const std::vector<std::vector<int>>& matrix1,
                            const std::vector<std::vector<int>>& matrix2);
std::vector<std::vector<int>> par_ribbon_horizontal_scheme
                            (const std::vector<std::vector<int>>& matrix1,
                            const std::vector<std::vector<int>>& matrix2);


#endif  // TASKS_TASK_2_SARATOVA_M_RIBBON_HORIZONTAL_SCHEME_RIBBON_HORIZONTAL_SCHEME_H_
