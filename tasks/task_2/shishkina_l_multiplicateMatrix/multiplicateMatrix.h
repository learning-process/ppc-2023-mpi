// Copyright 2023 Shishkina Valeria
#ifndef TASKS_TASK_2_SHISHKINA_L_MULTIPLICATEMATRIX_MULTIPLICATEMATRIX_H_
#define TASKS_TASK_2_SHISHKINA_L_MULTIPLICATEMATRIX_MULTIPLICATEMATRIX_H_

#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <vector>

std::vector<int> matrixMultiplicate(std::vector<int>* A, std::vector<int>* B,
                                    std::vector<int>* C, int maxSize,
                                    int count);
std::vector<int> matrixMultiplicationParallel(std::vector<int>* A,
                                              std::vector<int>* B,
                                              std::vector<int>* C, int maxSize);

#endif  // TASKS_TASK_2_SHISHKINA_L_MULTIPLICATEMATRIX_MULTIPLICATEMATRIX_H_
