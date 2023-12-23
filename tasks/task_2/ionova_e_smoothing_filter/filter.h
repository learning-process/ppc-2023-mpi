// Copyright 2023 Ionova Ekaterina
#ifndef TASKS_TASK_2_IONOVA_E_SMOOTHING_FILTER_FILTER_H_
#define TASKS_TASK_2_IONOVA_E_SMOOTHING_FILTER_FILTER_H_

#include <mpi.h>

#include <algorithm>
#include <random>
#include <vector>

std::vector<uint8_t> seqFilter(std::vector<uint8_t> img, int rows, int cols,
                               int kernel);
std::vector<uint8_t> parFilter(std::vector<uint8_t> img, int rows, int cols,
                               int kernel, MPI_Comm comm);
void unionImage(const std::vector<uint8_t>& subImg, std::vector<uint8_t>* img,
                int* ind);

#endif  // TASKS_TASK_2_IONOVA_E_SMOOTHING_FILTER_FILTER_H_
