// Copyright 2023 Smirnova Daria
#ifndef TASKS_TASK_2_SMIRNOVA_D_SIMPLE_ITERATION_SIMPLE_ITERATION_H_
#define TASKS_TASK_2_SMIRNOVA_D_SIMPLE_ITERATION_SIMPLE_ITERATION_H_

#include <vector>

std::vector<double> generate_random_Matrix(int size);
std::vector<double> simple_iterations(std::vector<double> delta_a, std::vector<double> x, double error,
                                    int size, int rank, int row_count, int size_proc);
bool is_equal(std::vector<double> x, std::vector<double> y);
std::vector<double> get_result(std::vector<double> matrix, int size, double error);

#endif  // TASKS_TASK_2_SMIRNOVA_D_SIMPLE_ITERATION_SIMPLE_ITERATION_H_
