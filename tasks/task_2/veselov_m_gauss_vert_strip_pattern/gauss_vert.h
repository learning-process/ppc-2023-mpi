// Copyright 2023 Veselov Mihail
#ifndef TASKS_TASK_2_VESELOV_M_GAUSS_VERT_STRIP_PATTERN_GAUSS_VERT_H_
#define TASKS_TASK_2_VESELOV_M_GAUSS_VERT_STRIP_PATTERN_GAUSS_VERT_H_

#include <mpi.h>
#include <cassert>
#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include <vector>
#include <stdexcept>

bool checkEqualOfMatrix(const std::vector <double> &firstMatrix,
                        const std::vector <double> &secondMatrix);
std::vector <double> nonParSolution(const std::vector <double> &coefs,
                                    size_t rows, size_t columns);
std::vector <double> ParSolution(const std::vector <double> &coefs,
                                size_t rows, size_t columns);
bool checkSolution(const std::vector <double> &coefs, size_t rows, size_t columns,
                    const std::vector <double> &xAns);

#endif  // TASKS_TASK_2_VESELOV_M_GAUSS_VERT_STRIP_PATTERN_GAUSS_VERT_H_
