// Copyright 2023 Kasimtcev Roman
#ifndef TASKS_TASK_3_KASIMTCEV_BLOCK_LINEAR_FILTERING_H_
#define TASKS_TASK_3_KASIMTCEV_BLOCK_LINEAR_FILTERING_H_

#include <mpi.h>

#include <algorithm>
#include <cstring>
#include <ctime>
#include <random>
#include <stdexcept>
#include <vector>

const int r = 5;
const int sizeG = 5;
const int gaussianKernel[] = {1, 2, 3, 4, 5, 6, 7, 7, 8};
const int norma = 15;

std::vector<double> randomImage(int row, int col);
std::vector<double> consistentOparation(std::vector<double> image, int row,
                                            int col);
std::vector<double> parllOp(std::vector<double> global_image,
                                          int row, int col);

#endif  // TASKS_TASK_3_KASIMTCEV_BLOCK_LINEAR_FILTERING_H_
