// Copyright 2023 Kostin Artem
#ifndef TASKS_TASK_3_KOSTIN_A_FOX_ALGORITHM_FOX_ALGORITHM_H_
#define TASKS_TASK_3_KOSTIN_A_FOX_ALGORITHM_FOX_ALGORITHM_H_

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <string>

std::vector<double> SequentialMul(std::vector<double> matrixa, std::vector<double> matrixb, int n);

void getRandMatrix(std::vector<double>* matrix, int N, int n);

bool isMatrEqual(std::vector<double> matrixa, std::vector<double> matrixb, int n);

void matrMalloc(double** matrix, int n);

void matrCalloc(double** matrix, int n);

void print_matr(std::vector<double> matrix, int Size);

std::vector<double> Fox_algorithm(std::vector<double> matrixa, std::vector<double> matrixb, int n);

#endif  // TASKS_TASK_3_KOSTIN_A_FOX_ALGORITHM_FOX_ALGORITHM_H_
