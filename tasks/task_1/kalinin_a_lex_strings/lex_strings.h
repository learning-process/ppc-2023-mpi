// Copyright 2023 Kalinin Alexandr
#ifndef TASKS_TASK_1_KALININ_A_LEX_STRINGS_LEX_STRINGS_H_
#define TASKS_TASK_1_KALININ_A_LEX_STRINGS_LEX_STRINGS_H_
#include <mpi.h>

#include <string>
#include <vector>
#include <algorithm>

bool checkOrder(char* str1, char* str2);
bool parallelLexicographicStrings(char* str1, char* str2);

#endif  // TASKS_TASK_1_KALININ_A_LEX_STRINGS_LEX_STRINGS_H_
