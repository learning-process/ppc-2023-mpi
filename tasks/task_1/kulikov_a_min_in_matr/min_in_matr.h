// Copyright 2023 Kulikov Artem
#ifndef TASKS_TASK_1_KULIKOV_A_MIN_IN_MATR_MIN_IN_MATR_H_
#define TASKS_TASK_1_KULIKOV_A_MIN_IN_MATR_MIN_IN_MATR_H_

#include <vector>
#include <boost/serialization/vector.hpp>

std::vector<int> getRandomMatrix(int m, int n);
std::vector<size_t> getParallelMin(std::vector<int> glob_matr, int m, int n);
std::vector<size_t> getSequentialMin(std::vector<int> matr, int m);

#endif  // TASKS_TASK_1_KULIKOV_A_MIN_IN_MATR_MIN_IN_MATR_H_
