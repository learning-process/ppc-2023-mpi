// Copyright 2023 Pozdnyakov Vasya
#ifndef TASKS_TASK_1_POZDNYAKOV_A_MAX_OF_VECTOR_ELEMENTS_MAX_OF_VECTOR_ELEMENTS_H_
#define TASKS_TASK_1_POZDNYAKOV_A_MAX_OF_VECTOR_ELEMENTS_MAX_OF_VECTOR_ELEMENTS_H_

#include <vector>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>

std::vector<int> get_random_vector(size_t size, int min_elem = -1000, int max_elem = 1000);
int get_max_element(const std::vector<int>& init_vec, size_t vec_size);

#endif  // TASKS_TASK_1_POZDNYAKOV_A_MAX_OF_VECTOR_ELEMENTS_MAX_OF_VECTOR_ELEMENTS_H_
