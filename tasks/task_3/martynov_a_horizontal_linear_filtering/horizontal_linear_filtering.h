// Copyright 2023 Martynov Aleksandr
#ifndef TASKS_TASK_3_MARTYNOV_A_HORIZONTAL_LINEAR_FILTERING_HORIZONTAL_LINEAR_FILTERING_H_
#define TASKS_TASK_3_MARTYNOV_A_HORIZONTAL_LINEAR_FILTERING_HORIZONTAL_LINEAR_FILTERING_H_

#include <mpi.h>
#include <vector>
#include <limits>
#include <random>

std::vector<unsigned char> GetImage(int rows, int cols);
std::vector<unsigned char> toFiltA(const std::vector<unsigned char>& image,
  int rows, int cols);
std::vector<unsigned char> toFiltB(const std::vector<unsigned char>& image,
  int rows, int cols);

#endif  // TASKS_TASK_3_MARTYNOV_A_HORIZONTAL_LINEAR_FILTERING_HORIZONTAL_LINEAR_FILTERING_H_
