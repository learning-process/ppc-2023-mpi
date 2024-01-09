// Copyright 2023 Pushkarev Ilya

#ifndef TASKS_TASK_3_PUSHKAREV_I_SHORTEST_PATH_MOORE_SHORTEST_PATH_MOORE_H_
#define TASKS_TASK_3_PUSHKAREV_I_SHORTEST_PATH_MOORE_SHORTEST_PATH_MOORE_H_
#include <vector>
#include <algorithm>

const int INF = INT_MAX / 2;

std::vector<int> mooore(int my_rank, int p, int n, const std::vector<int>& mat, bool* has_negative_cycle);
#endif  // TASKS_TASK_3_PUSHKAREV_I_SHORTEST_PATH_MOORE_SHORTEST_PATH_MOORE_H_

