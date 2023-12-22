// Copyright 2023 Zawadowski Jan
#ifndef TASKS_TASK_3_ZAWADOWSKI_J_HORIZONTAL_LINEAR_FILTERING_HORIZONTAL_LINEAR_FILTERING_H_
#define TASKS_TASK_3_ZAWADOWSKI_J_HORIZONTAL_LINEAR_FILTERING_HORIZONTAL_LINEAR_FILTERING_H_

#include <mpi.h>
#include <vector>
#include <limits>
#include <random>

std::vector<unsigned char> getRandomImage(int rows, int cols);
std::vector<unsigned char> filterImageP(const std::vector<unsigned char> &image,
                                        int rows, int cols);
std::vector<unsigned char> filterImageC(const std::vector<unsigned char> &image,
                                        int rows, int cols);

#endif  // TASKS_TASK_3_ZAWADOWSKI_J_HORIZONTAL_LINEAR_FILTERING_HORIZONTAL_LINEAR_FILTERING_H_
