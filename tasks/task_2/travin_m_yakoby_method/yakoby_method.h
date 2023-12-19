// Copyright 2023 Travin Maksim
#ifndef TASKS_TASK_2_TRAVIN_M_YAKOBY_METHOD_YAKOBY_METHOD_H_
#define TASKS_TASK_2_TRAVIN_M_YAKOBY_METHOD_YAKOBY_METHOD_H_
#include <vector>

std::vector<double> Yakobi(const std::vector<double>& A,
    const std::vector<double>& b,
    int num_iterations = 10);
std::vector<double> get_rand_matrix(int size,
    int min = -1000, int max = -1000);
std::vector<double> get_rand_vector(int size, int min = -1000, int max = 1000);
#endif  // TASKS_TASK_2_TRAVIN_M_YAKOBY_METHOD_YAKOBY_METHOD_H_
