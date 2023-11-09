// Copyright 2023 Kulaev Eugene
#ifndef TASKS_TASK_2_KULAEV_E_INCREASING_CONTRAST_INCREASING_CONTRAST_H_
#define TASKS_TASK_2_KULAEV_E_INCREASING_CONTRAST_INCREASING_CONTRAST_H_

#include <algorithm>
#include <vector>

#include <boost/mpi.hpp>
#include <boost/algorithm/clamp.hpp>


void Stretching(std::vector<int>* local_img, int global_min,
                        int global_max, int new_min, int new_max);
int generateRandomNumbers(int min, int max);
int SeqmetionMax(std::vector<int>* vec);
int SeqmetionMin(std::vector<int>* vec);
void ParallelStretching(std::vector<int>* image, int new_min,
                                                int new_max);

#endif  // TASKS_TASK_2_KULAEV_E_INCREASING_CONTRAST_INCREASING_CONTRAST_H_
