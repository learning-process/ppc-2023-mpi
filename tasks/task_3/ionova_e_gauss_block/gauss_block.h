// Copyright 2023 Ionova Ekaterina
#ifndef TASKS_TASK_3_IONOVA_E_GAUSS_BLOCK_GAUSS_BLOCK_H_
#define TASKS_TASK_3_IONOVA_E_GAUSS_BLOCK_GAUSS_BLOCK_H_

#include <mpi.h>

#include <algorithm>
#include <cstring>
#include <ctime>
#include <random>
#include <stdexcept>
#include <vector>

const int gaussianRadius = 1;
const int gaussianSize = 3;
const int gaussianKernel[] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
const int gaussianNorm = 16;

std::vector<double> getRandomImage(int rows, int cols);
std::vector<double> getSequentialOperations(std::vector<double> image, int rows,
                                            int cols);
std::vector<double> getParallelOperations(std::vector<double> global_image,
                                          int rows, int cols);

#endif  // TASKS_TASK_3_IONOVA_E_GAUSS_BLOCK_GAUSS_BLOCK_H_
