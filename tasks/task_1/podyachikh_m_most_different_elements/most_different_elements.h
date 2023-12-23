// Copyright 2023 Podyachikh Mikhail
#ifndef TASKS_TASK_1_PODYACHIKH_M_MOST_DIFFERENT_ELEMENTS_MOST_DIFFERENT_ELEMENTS_H_
#define TASKS_TASK_1_PODYACHIKH_M_MOST_DIFFERENT_ELEMENTS_MOST_DIFFERENT_ELEMENTS_H_

#include <vector>

std::vector<int> VectorRandomNumbers(int vector_size, int min_value, int max_value);
int SequentialGetMostDifferentElements(const std::vector<int> &vec);
int ParallelGetMostDifferentElements(const std::vector<int> &vec);

#endif  // TASKS_TASK_1_PODYACHIKH_M_MOST_DIFFERENT_ELEMENTS_MOST_DIFFERENT_ELEMENTS_H_
