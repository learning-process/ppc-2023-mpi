// Copyright 2023 Pushkarev Ilya

#ifndef TASKS_TASK_1_PUSHKAREV_I_MOST_DIFFERENT_NEIGHBOR_ELEMENTS_MOST_DIFFERENT_NEIGHBOR_ELEMENTS_H_
#define TASKS_TASK_1_PUSHKAREV_I_MOST_DIFFERENT_NEIGHBOR_ELEMENTS_MOST_DIFFERENT_NEIGHBOR_ELEMENTS_H_

#include <mpi.h>
#include <vector>

int maxDifferenceSeq(const std::vector<int> &vec);
int maxDifferencePar(const std::vector<int> &vec);
std::vector<int> create_random_vector(size_t size, int lower_bound, int upper_bound);

#endif //TASKS_TASK_1_PUSHKAREV_I_MOST_DIFFERENT_NEIGHBOR_ELEMENTS_MOST_DIFFERENT_NEIGHBOR_ELEMENTS_H_
