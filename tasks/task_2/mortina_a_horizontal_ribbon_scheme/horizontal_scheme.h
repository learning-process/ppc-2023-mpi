// Copyright 2023 Mortina Anastasia
#ifndef TASKS_TASK_2_MORTINA_A_HORIZONTAL_RIBBON_SCHEME_HORIZONTAL_SCHEME_H_
#define TASKS_TASK_2_MORTINA_A_HORIZONTAL_RIBBON_SCHEME_HORIZONTAL_SCHEME_H_


#include <mpi.h>
#include <vector>
#include <iostream>
#include <random>

std::vector<int> generateMatrixRandom(int rows, int cols, int min, int max);
int generateRandomNumbers(int min, int max);
void matrixVectorMultiply(const std::vector<int>& matrix, const std::vector<int>& vector,
                    std::vector<int>* result, int cols, int start, int end);
std::vector<int> ParallMatrixVector(const std::vector<int>& matrix, const std::vector<int>& vector, int rows, int cols);



#endif  // TASKS_TASK_2_MORTINA_A_HORIZONTAL_RIBBON_SCHEME_HORIZONTAL_SCHEME_H_
