// Copyright 2023 Shemyakina Alesya
#ifndef TASKS_TASK_3_SHEMYAKINA_A_STRASSEN_ALGORITHM_STRASSEN_ALGORITHM_H_
#define TASKS_TASK_3_SHEMYAKINA_A_STRASSEN_ALGORITHM_STRASSEN_ALGORITHM_H_

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <random>
#include <algorithm>
#include <functional>
#include <iostream>
#include <utility>

#include <string>

std::vector<double> sequentialMul(std::vector<double> matrixa,
 std::vector<double> matrixb, int n);

void getRandMatrix(std::vector<double>* matrix, int N, int n);

bool MatrixEqual(std::vector<double> matrixa,
 std::vector<double> matrixb, int n);

std::vector<double> StrassenAlgorithm(std::vector<double> matrixa,
 std::vector<double> matrixb, int n);

#endif  // TASKS_TASK_3_SHEMYAKINA_A_STRASSEN_ALGORITHM_STRASSEN_ALGORITHM_H_
