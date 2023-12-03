// Copyright 2023 Saratova Marina
#ifndef TASKS_TASK_1_SARATOVA_M_MIN_ELEM_VECTOR_MIN_ELEM_VECTOR_H_
#define TASKS_TASK_1_SARATOVA_M_MIN_ELEM_VECTOR_MIN_ELEM_VECTOR_H_

#include <mpi.h>

#include <random>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>


std::vector<int> generate_random_vector(int size, int low, int high);
int seq_find_min_elem_vector(const std::vector<int>& vec);
int par_find_min_elem_vector(const std::vector<int>& vec);

#endif  // TASKS_TASK_1_SARATOVA_M_MIN_ELEM_VECTOR_MIN_ELEM_VECTOR_H_
