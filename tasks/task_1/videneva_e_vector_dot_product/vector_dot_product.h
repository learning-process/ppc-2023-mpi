// Copyright 2023 Videneva Ekaterina
#ifndef TASKS_TASK_1_VIDENEVA_E_VECTOR_DOT_PRODUCT_VECTOR_DOT_PRODUCT_H_
#define TASKS_TASK_1_VIDENEVA_E_VECTOR_DOT_PRODUCT_VECTOR_DOT_PRODUCT_H_

#include <mpi.h>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <random>


int scalar_product(const std::vector<int>& vec1, const std::vector<int>& vec2);
int Parall_product(const std::vector<int>& vec1, const std::vector<int>& vec2);


#endif  // TASKS_TASK_1_VIDENEVA_E_VECTOR_DOT_PRODUCT_VECTOR_DOT_PRODUCT_H_
