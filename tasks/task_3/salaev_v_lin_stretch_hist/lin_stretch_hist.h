// Copyright 2023 Salaev Vladislav
#ifndef TASKS_TASK_3_SALAEV_V_LIN_STRETCH_HIST_LIN_STRETCH_HIST_H_
#define TASKS_TASK_3_SALAEV_V_LIN_STRETCH_HIST_LIN_STRETCH_HIST_H_
#include <mpi.h>

#include <vector>
#include <algorithm>
#include <random>

std::vector<uint8_t> generateImage(
size_t width,
size_t height, uint8_t minVal, uint8_t maxVal);
void increaseContrast(
std::vector<uint8_t>* img,
uint8_t oldMin, uint8_t oldMax, uint8_t newMin, uint8_t newMax);
void parallelIncreaseContrast(
std::vector<uint8_t>* img,
size_t width, size_t height, uint8_t newMin, uint8_t newMax, MPI_Comm comm);

#endif  // TASKS_TASK_3_SALAEV_V_LIN_STRETCH_HIST_LIN_STRETCH_HIST_H_
