// Copyright 2023 Kiselev Igor
#ifndef TASKS_TASK_1_KISELEV_I_STRASSEN_MUL_STRASSEN_MUL_H
#define TASKS_TASK_1_KISELEV_I_STRASSEN_MUL_STRASSEN_MUL_H
#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

vector<vector<double>> matrix_addition(const vector<vector<double>>& A, const vector<vector<double>>& B);
vector<vector<double>> matrix_subtraction(const vector<vector<double>>& A, const vector<vector<double>>& B);
vector<vector<double>> matrix_multiplication(const vector<vector<double>>& A, const vector<vector<double>>& B);
void subdivide_matrix(const vector<vector<double>>& A,
                      vector<vector<double>>& A11,
                      vector<vector<double>>& A12,
                      vector<vector<double>>& A21,
                      vector<vector<double>>& A22);
vector<vector<double>> merge_matrix(const vector<vector<double>>& C11,
                                              const vector<vector<double>>& C12,
                                              const vector<vector<double>>& C21,
                                              const vector<vector<double>>& C22);
vector<vector<double>> strassen_mult(const vector<vector<double>>& A,
                                               const vector<vector<double>>& B);
#endif // TASKS_TASK_1_KISELEV_I_STRASSEN_MUL_STRASSEN_MUL_H
