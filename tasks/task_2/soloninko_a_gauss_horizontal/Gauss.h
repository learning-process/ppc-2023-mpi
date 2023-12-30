// Copyright 2023 Soloninko Andrey
#ifndef TASKS_TASK_2_SOLONINKO_A_GAUSS_HORIZONTAL_GAUSS_H_
#define TASKS_TASK_2_SOLONINKO_A_GAUSS_HORIZONTAL_GAUSS_H_

#include <mpi.h>

#include <iostream>
#include <random>
#include <vector>

bool check_res(std::vector<double>* matrix, std::vector<double>* x,
               int size);

void rand_matr(std::vector<double>* matrix, int size);

void gaussian_elimination(std::vector<double>* matrix, std::vector<int>* rows,
                          int size, int rank, int comm_size, int row,
                          int numsegment);

void gaussian_substitution(const std::vector<double>& matrix,
                           std::vector<double>* res,
                           const std::vector<int>& rows, int row, int nrows,
                           int rank, int comm_size, int size);

void gauss_algo(std::vector<double>* matrix, std::vector<double>* res,
                int size);

#endif  // TASKS_TASK_2_SOLONINKO_A_GAUSS_HORIZONTAL_GAUSS_H_
