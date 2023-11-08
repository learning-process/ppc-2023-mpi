// Copyright 2023 Kulaev Eugene
#ifndef TASKS_TASK_2_KULAEV_E_CONTRAST_ENHANCEMENT_CONTRAST_ENHA_H_
#define TASKS_TASK_2_KULAEV_E_CONTRAST_ENHANCEMENT_CONTRAST_ENHA_H_

#include <algorithm>
#include <vector>

#include <boost/mpi.hpp>
#include <boost/algorithm/clamp.hpp>


void Seq_linearStretchingHistogram(std::vector<int>* local_img, int global_min,
                        int global_max, int new_min, int new_max);
int Par_linearStretchingHistogram(std::vector<int>* image, int new_min,
                                                int new_max, int n, int m);
int generateRandomNumbers(int min, int max);

#endif  // TASKS_TASK_2_KULAEV_E_CONTRAST_ENHANCEMENT_CONTRAST_ENHA_H_
