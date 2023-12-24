// Copyright 2023 Sobol Lubov

#ifndef TASKS_TASK_2_SOBOL_L_HORIZONTAL_RIBBON_SCHEME_HORIZONTAL_SCHEME_H_
#define TASKS_TASK_2_SOBOL_L_HORIZONTAL_RIBBON_SCHEME_HORIZONTAL_SCHEME_H_

#include <mpi.h>
#include <vector>
#include <iostream>
#include <random>

std::vector<int> generateRandomMatrix(int rows, int cols, int min, int max);
int generateSingleRandomNumber(int min, int max);
void multiplyMatrixVector(const std::vector<int>& matrix, const std::vector<int>& vector,
                    std::vector<int>* result, int cols, int start, int end);
std::vector<int> parallelMatrixVector(const std::vector<int>& matrix,
				      const std::vector<int>& vector, int rows, int cols);

#endif  // TASKS_TASK_2_SOBOL_L_HORIZONTAL_RIBBON_SCHEME_HORIZONTAL_SCHEME_H_
