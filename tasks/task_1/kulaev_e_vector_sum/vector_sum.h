// Copyright 2023 Kulaev Eugene
#ifndef TASKS_TASK_1_KULAEV_E_VECTOR_SUM_VECTOR_SUM_H_
#define TASKS_TASK_1_KULAEV_E_VECTOR_SUM_VECTOR_SUM_H_

#include <mpi.h>
#include <vector>

int sumSeq(const std::vector<int> &vec);
int sumPar(const std::vector<int> &vec);
int generateRandomNumbers(int min, int max);

#endif  // TASKS_TASK_1_KULAEV_E_VECTOR_SUM_VECTOR_SUM_H_
