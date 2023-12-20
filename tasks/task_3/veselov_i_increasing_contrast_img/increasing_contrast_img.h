// Copyright 2023 Veselov Ilya
#ifndef TASKS_TASK_3_VESELOV_I_INCREASING _CONTRAST_IMG_INCREASING_CONTRAST_IMG_H_
#define TASKS_TASK_3_VESELOV_I_INCREASING _CONTRAST_IMG_INCREASING_CONTRAST_IMG_H_

#include <mpi.h>
#include <vector>
//#include <algorithm>
#include <random>

std::vector<uint8_t> generateImg(size_t width, size_t height, uint8_t min_, uint8_t max_);
void incContrastImg(std::vector<uint8_t>* img, uint8_t oldMin,
                     uint8_t oldMax, uint8_t newMin, uint8_t newMax);
void parIncContrastImg(std::vector<uint8_t>* img, size_t width, size_t height,
                     uint8_t newMin, uint8_t newMax, MPI_Comm comm);

#endif  // TASKS_TASK_3_VESELOV_I_INCREASING _CONTRAST_IMG_INCREASING_CONTRAST_IMG_H_
