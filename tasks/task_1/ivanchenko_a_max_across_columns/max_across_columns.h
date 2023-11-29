// Copyright 2023 Ivanchenko Aleksei
#ifndef TASKS_TASK_1_IVANCHENKO_MAX_ACROSS_COLUMNS_
#define TASKS_TASK_1_IVANCHENKO_MAX_OF_MATRIX_

#include <vector>
#include <string>

std::vector<int> getRandomVector(int  sz);
int getParallelOperations(std::vector<int> global_vec, int rows, int columns);
int getSequentialOperations(std::vector<int> vec, int rows, int columns);

#endif  // TASKS_TASK_1_IVANCHENKO_MAX_ACROSS_COLUMNS_
