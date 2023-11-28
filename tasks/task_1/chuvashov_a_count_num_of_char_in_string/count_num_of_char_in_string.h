// Copyright 2023 Chuvashov Andrey

#ifndef TASKS_TASK_1_CHUVASHOV_A_COUNT_NUM_OF_CHAR_IN_STRING_COUNT_NUM_OF_CHAR_IN_STRING_H_
#define TASKS_TASK_1_CHUVASHOV_A_COUNT_NUM_OF_CHAR_IN_STRING_COUNT_NUM_OF_CHAR_IN_STRING_H_

#include <mpi.h>
#include <time.h>
#include <iostream>
#include <random>
#include <vector>
#include <string>

std::string GenerateRandomString(size_t strSize);
std::string GenerateRandomOnlyLettersString(size_t strSize);
int ParallelLettersCount(const std::string& str);
int SequentialLettersCount(const std::string& str);

#endif  // TASKS_TASK_1_CHUVASHOV_A_COUNT_NUM_OF_CHAR_IN_STRING_COUNT_NUM_OF_CHAR_IN_STRING_H_
