// Copyright 2023 Kiselev Igor
#ifndef TASKS_TASK_1_KISELEV_I_VECTOR_SUM_VECTOR_SUM_H_
#define TASKS_TASK_1_KISELEV_I_VECTOR_SUM_VECTOR_SUM_H_

#include <mpi.h>
#include <vector>

int sumSeq(const std::vector<int> &vec);
int sumPar(const std::vector<int> &vec);
int generateRandomNumbers(int min, int max);

#endif  // TASKS_TASK_1_KISELEV_I_VECTOR_SUM_VECTOR_SUM_H_
