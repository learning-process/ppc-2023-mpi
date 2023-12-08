// Copyright 2023 Vetoshnikova Ekaterina
#ifndef TASKS_TASK_1_VETOSHNIKOVA_K_VECTOR_DOT_PRODUCT_VECTOR_DOT_PRODUCT_H_
#define TASKS_TASK_1_VETOSHNIKOVA_K_VECTOR_DOT_PRODUCT_VECTOR_DOT_PRODUCT_H_

#include <vector>
#include <mpi.h>

std::vector<int> vector_generation(int mn, int mx, int sz);
int getParallelOperations(std::vector<int> global_vec_a, std::vector<int> global_vec_b,
	                      int count_size_vector);
int getSequentialOperations(std::vector<int> a, std::vector<int> b,
	int count_size_vector);

#endif  // TASKS_TASK_1_VETOSHNIKOVA_K_VECTOR_DOT_PRODUCT_VECTOR_DOT_PRODUCT_H_
