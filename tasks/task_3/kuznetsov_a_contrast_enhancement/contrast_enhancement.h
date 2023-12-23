// Copyright 2023 Kuznetsov Artem
#ifndef TASKS_TASK_3_KUZNETSOV_A_CONTRAST_ENHANCEMENT_CONTRAST_ENHANCEMENT_H_
#define TASKS_TASK_3_KUZNETSOV_A_CONTRAST_ENHANCEMENT_CONTRAST_ENHANCEMENT_H_

#include <mpi.h>

#include <algorithm>
#include <random>
#include <vector>

std::vector<uint8_t> create_random_image(size_t m, size_t n, uint8_t min,
                                         uint8_t max);
void seq_increase_contrast(std::vector<uint8_t>* image, uint8_t old_min,
                           uint8_t old_max, uint8_t new_min, uint8_t new_max);
void par_increase_contrast(std::vector<uint8_t>* image, size_t m, size_t n,
                           uint8_t new_min, uint8_t new_max, MPI_Comm comm);

#endif  // TASKS_TASK_3_KUZNETSOV_A_CONTRAST_ENHANCEMENT_CONTRAST_ENHANCEMENT_H_
