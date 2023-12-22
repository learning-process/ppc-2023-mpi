// Copyright 2023 Ivanov Nikita
#ifndef TASKS_TASK_2_IVANOV_GAUSS_LENT_HORIZ_GAUSS_LENT_HORIZ_H_
#define TASKS_TASK_2_IVANOV_GAUSS_LENT_HORIZ_GAUSS_LENT_HORIZ_H_
#include <vector>

std::vector<double> Gauss(std::vector<double> matrix, int size);

bool check_result(std::vector<double> matrix, std::vector<double> x, int size);

std::vector<double> create_random_matrix(int size);

#endif  // TASKS_TASK_2_IVANOV_GAUSS_LENT_HORIZ_GAUSS_LENT_HORIZ_H_
