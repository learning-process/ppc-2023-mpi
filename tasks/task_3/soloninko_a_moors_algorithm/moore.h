// Copyright 2023 Soloninko Andrey
#ifndef TASKS_TASK_3_SOLONINKO_A_MOORS_ALGORITHM_MOORE_H_
#define TASKS_TASK_3_SOLONINKO_A_MOORS_ALGORITHM_MOORE_H_

#include <mpi.h>

#include <iostream>
#include <random>
#include <vector>

std::vector<int> rand_matr(int size);

void bellman_moore_parallel(int n, std::vector<int>* mat,
                            std::vector<int>* dist, bool has_negative_cycle);

void bellman_moore(int n, std::vector<int>* mat, std::vector<int>* dist,
                   bool has_negative_cycle);

#endif  // TASKS_TASK_3_SOLONINKO_A_MOORS_ALGORITHM_MOORE_H_
