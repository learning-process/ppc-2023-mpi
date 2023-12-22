// Copyright 2023 Moiseev Nikita
#ifndef TASKS_TASK_1_MOISEEV_N_NUM_OF_ALTERNATIONS_SIGNS_NUM_OF_ALTERNATIONS_SIGNS_H_
#define TASKS_TASK_1_MOISEEV_N_NUM_OF_ALTERNATIONS_SIGNS_NUM_OF_ALTERNATIONS_SIGNS_H_

#include <mpi.h>
#include <random>
#include <vector>

std::vector<int> create_random_array(size_t size);
int seq_num_of_alternations(const std::vector<int>& arr);
int par_num_of_alternations(const std::vector<int>& arr);

#endif  // TASKS_TASK_1_MOISEEV_N_NUM_OF_ALTERNATIONS_SIGNS_NUM_OF_ALTERNATIONS_SIGNS_H_
