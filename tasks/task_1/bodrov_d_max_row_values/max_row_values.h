// Copyright 2023 Bodrov Daniil
#ifndef TASKS_TASK_1_BODROV_D_MAX_ROW_VALUES_MAX_ROW_VALUES_H_
#define TASKS_TASK_1_BODROV_D_MAX_ROW_VALUES_MAX_ROW_VALUES_H_
#include <mpi.h>
#include <vector>

std::vector<int> FindMaxRowValues(const std::vector<int>& matrix, int n);
std::vector<int> FindMaxRowValuesPar(const std::vector<int>& matrix, int n);


#endif  // TASKS_TASK_1_BODROV_D_MAX_ROW_VALUES_MAX_ROW_VALUES_H_
