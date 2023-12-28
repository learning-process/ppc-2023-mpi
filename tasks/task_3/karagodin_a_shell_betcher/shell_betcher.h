// Copyright 2023 Karagodin Andrey
#ifndef TASKS_TASK_3_KARAGODIN_A_SHELL_BETCHER_SHELL_BETCHER_H_
#define TASKS_TASK_3_KARAGODIN_A_SHELL_BETCHER_SHELL_BETCHER_H_

#include <mpi.h>
#include <algorithm>
#include <random>
#include <vector>
#include <string>
#include <utility>

std::vector<int> ShellSortSequential(std::vector<int> arr);
std::vector<int> ShellSortParallel(const std::vector<int>& arr, int arrSize);
std::vector<int> GenerateArrayRandom(int arrSize);

#endif  // TASKS_TASK_3_KARAGODIN_A_SHELL_BETCHER_SHELL_BETCHER_H_
