//  Copyright 2023 Ryabkov Vladislav

#ifndef TASKS_TASK_2_RYABKOV_V_VERTICAL_STRIP_STRIP_VERTICAL_SCHEME_H_
#define TASKS_TASK_2_RYABKOV_V_VERTICAL_STRIP_STRIP_VERTICAL_SCHEME_H_

#include <mpi.h>
#include <algorithm>
#include <random>
#include <vector>

int* init_empty_matrix(int rows);
int* get_random_matrix(int rows, int columns);
void sequential_matrix_multiplication(const int* A, const int A_rows,
                                      const int A_columns, const int* B,
                                      const int B_rows, int* res);
void parallel_matrix_multiplication(const int* A, const int A_rows,
                                    const int A_columns, const int* B,
                                    const int B_rows, int* res);

#endif  // TASKS_TASK_2_RYABKOV_V_VERTICAL_STRIP_STRIP_VERTICAL_SCHEME_H_
