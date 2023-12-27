// Copyright 2023 Kiselev Igor
#ifndef TASKS_TASK_3_KISELEV_I_STRASSEN_MUL_STRASSEN_MUL_H_
#define TASKS_TASK_3_KISELEV_I_STRASSEN_MUL_STRASSEN_MUL_H_
#include <mpi.h>
#include <iostream>
#include <vector>
#include <boost/core/invoke_swap.hpp>

std::vector<std::vector<double>> matrix_addition(const std::vector<std::vector<double>>& A, const std::vector<std::vector<double>>& B);
std::vector<std::vector<double>> matrix_subtraction(const std::vector<std::vector<double>>& A, const std::vector<std::vector<double>>& B);
std::vector<std::vector<double>> matrix_multiplication(const std::vector<std::vector<double>>& A, const std::vector<std::vector<double>>& B);
void subdivide_matrix(const std::vector<std::vector<double>>& A,
                      std::vector<std::vector<double>>& A11,
                      std::vector<std::vector<double>>& A12,
                      std::vector<std::vector<double>>& A21,
                      std::vector<std::vector<double>>& A22);
std::vector<std::vector<double>> merge_matrix(const std::vector<std::vector<double>>& C11,
                                              const std::vector<std::vector<double>>& C12,
                                              const std::vector<std::vector<double>>& C21,
                                              const std::vector<std::vector<double>>& C22);
std::vector<std::vector<double>> strassen_mult(const std::vector<std::vector<double>>& A,
                                               const std::vector<std::vector<double>>& B);
#endif // TASKS_TASK_3_KISELEV_I_STRASSEN_MUL_STRASSEN_MUL_H_
