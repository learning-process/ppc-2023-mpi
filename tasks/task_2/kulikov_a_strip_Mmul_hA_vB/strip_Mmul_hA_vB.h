// Copyright 2023 Kulikov Artem
#ifndef TASKS_TASK_2_KULIKOV_A_STRIP_MMUL_HA_VB_STRIP_MMUL_HA_VB_H_
#define TASKS_TASK_2_KULIKOV_A_STRIP_MMUL_HA_VB_STRIP_MMUL_HA_VB_H_

#include <vector>
#include <boost/serialization/vector.hpp>

std::vector<int> getRandomMatrix(int n);
std::vector<int> getParallelMatrMul(std::vector<int> A, std::vector<int> B, int n);
std::vector<int> getSequentialMatrMul(std::vector<int> A, std::vector<int> B, int n);

#endif  // TASKS_TASK_2_KULIKOV_A_STRIP_MMUL_HA_VB_STRIP_MMUL_HA_VB_H_
