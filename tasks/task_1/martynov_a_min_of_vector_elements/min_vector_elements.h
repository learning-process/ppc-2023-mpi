// Copyright 2023 Martynov Aleksandr
#ifndef TASKS_TASK_1_MARTYNOV_A_MIN_OF_VECTOR_ELEMENTS_MIN_VECTOR_ELEMENTS_H_
#define TASKS_TASK_1_MARTYNOV_A_MIN_OF_VECTOR_ELEMENTS_MIN_VECTOR_ELEMENTS_H_

#include <vector>

int get_minimal_elem(const std::vector<int>& init_vec, size_t vec_size);

void get_random_vector(std::vector<int>* vector, size_t size,
  int bot_elem = -100, int top_elem = 100);

#endif  // TASKS_TASK_1_MARTYNOV_A_MIN_OF_VECTOR_ELEMENTS_MIN_VECTOR_ELEMENTS_H_
