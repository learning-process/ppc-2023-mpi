// Copyright 2024 Efimov Andrey

#ifndef TASKS_TASK_1_EFIMOV_A_NUMBER_OF_VIOLATIONS_IN_THE_VECTOR_NUMBER_OF_ORDERLY_VIOLATIONS_H_
#define TASKS_TASK_1_EFIMOV_A_NUMBER_OF_VIOLATIONS_IN_THE_VECTOR_NUMBER_OF_ORDERLY_VIOLATIONS_H_

#include <mpi.h>
#include <algorithm>
#include <vector>
#include <random>
#include <ctime>
#include <iostream>

int numberOfOrderValids_Sync(const std::vector<int>& Vector);

int numberOfOrderValids_ASync(const std::vector<int>& Vector, int Vector_len);

std::vector<int> make_random_vector(const int& Vector_len);

#endif  // TASKS_TASK_1_EFIMOV_A_NUMBER_OF_VIOLATIONS_IN_THE_VECTOR_NUMBER_OF_ORDERLY_VIOLATIONS_H_
