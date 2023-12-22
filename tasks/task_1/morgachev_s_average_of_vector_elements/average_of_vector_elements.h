// Copyright 2023 Morgachev Stepan
#ifndef TASKS_TASK_1_MORGACHEV_S_AVERAGE_OF_VECTOR_ELEMENTS_AVERAGE_OF_VECTOR_ELEMENTS_H_
#define TASKS_TASK_1_MORGACHEV_S_AVERAGE_OF_VECTOR_ELEMENTS_AVERAGE_OF_VECTOR_ELEMENTS_H_

#include <mpi.h>
#include <vector>
#include <random>

std::vector<int> fillVectorRandomNumbers(size_t size, int left, int right);
double calculateAverageOfVectorElements(const std::vector<int>& vector);
int calculatePartialSum(const std::vector<int>& vec);

#endif  // TASKS_TASK_1_MORGACHEV_S_AVERAGE_OF_VECTOR_ELEMENTS_AVERAGE_OF_VECTOR_ELEMENTS_H_
