// Copyright 2023 Kulaev Eugene
#ifndef TASKS_TASK_2_KULAEV_E_INCREASING_CONTRAST_INCREASING_CONTRAST_H_
#define TASKS_TASK_2_KULAEV_E_INCREASING_CONTRAST_INCREASING_CONTRAST_H_

#include <algorithm>
#include <vector>

#include <boost/mpi.hpp>
#include <boost/algorithm/clamp.hpp>


void Stretching(std::vector<uint8_t>* local_img, uint8_t global_min,
                        uint8_t global_max, uint8_t new_min, uint8_t new_max);
uint8_t generateRandomNumbers(uint8_t min, uint8_t max);
uint8_t SeqmetionMax(std::vector<uint8_t>* vec);
uint8_t SeqmetionMin(std::vector<uint8_t>* vec);
void ParallelStretching(std::vector<uint8_t>* image, uint8_t new_min,
                                                uint8_t new_max);

#endif  // TASKS_TASK_2_KULAEV_E_INCREASING_CONTRAST_INCREASING_CONTRAST_H_
