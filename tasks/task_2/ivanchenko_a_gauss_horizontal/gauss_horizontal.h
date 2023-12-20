// Copyright 2023 Ivanchenko Aleksei
#ifndef TASKS_TASK_2_IVANCHENKO_A_GAUSS_HORIZONTAL_GAUSS_HORIZONTAL_H_
#define TASKS_TASK_2_IVANCHENKO_A_GAUSS_HORIZONTAL_GAUSS_HORIZONTAL_H_

#include <vector>

std::vector<double>getRandomVector(size_t size, int minElem = 1, int maxElem = 1e8);
std::vector<double>getRandomMatrix(size_t rows, size_t columns, int minElem = 1, int maxElem = 1e8);
std::vector<double>gaussSequentional(std::vector<double>A,
 std::vector<double>b);
std::vector<double>gaussParallel(std::vector<double>A,
 std::vector<double>b);

#endif  // TASKS_TASK_2_IVANCHENKO_A_GAUSS_HORIZONTAL_GAUSS_HORIZONTAL_H_
