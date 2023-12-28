// Copyright 2023 Kochetov Nikolay
#ifndef TASKS_TASK_1_KOCHETOV_N_MAX_OF_VECTOR_ELEMENTS_MAX_OF_VECTOR_ELEMENTS_H_
#define TASKS_TASK_1_KOCHETOV_N_MAX_OF_VECTOR_ELEMENTS_MAX_OF_VECTOR_ELEMENTS_H_

#include <vector>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>

std::vector<int> GetRandomVector(
    size_t size, int min_elem = -1000, int max_elem = 1000);
int GetMaxElement(const std::vector<int>& init_vec, size_t vec_size);

#endif  // TASKS_TASK_1_KOCHETOV_N_MAX_OF_VECTOR_ELEMENTS_MAX_OF_VECTOR_ELEMENTS_H_
