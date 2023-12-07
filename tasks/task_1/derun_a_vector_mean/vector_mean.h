// Copyright 2023 Derun Andrew
#ifndef TASKS_TASK_1_DERUN_A_VECTOR_MEAN_VECTOR_MEAN_H_
#define TASKS_TASK_1_DERUN_A_VECTOR_MEAN_VECTOR_MEAN_H_

#include <mpi.h>

#include <vector>

double meanSeq(const std::vector<int> &vec);
double meanPar(const std::vector<int> &vec);
int generateRandomNumber(int min, int max);

#endif  // TASKS_TASK_1_DERUN_A_VECTOR_MEAN_VECTOR_MEAN_H_
