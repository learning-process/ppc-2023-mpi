// Copyright 2023 Bozin Dmitry
#ifndef TASKS_TASK_2_BOZIN_D_CONTRAST_CONTRAST_H_
#define TASKS_TASK_2_BOZIN_D_CONTRAST_CONTRAST_H_

#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>

std::vector<int> getRandomVector(int size);
std::vector<std::vector<int>> ParallelContrast(std::vector<std::vector<int>> vector, int weight, int height);
std::vector<std::vector<int>> SequentialContrast(std::vector<std::vector<int>> vector, int weight, int height);

#endif  // TASKS_TASK_2_BOZIN_D_CONTRAST_CONTRAST_H_
