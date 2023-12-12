// Copyright 2023 Kostin Artem
#ifndef TASKS_TASK_3_KOSTIN_A_FOX_ALGORITHM_FOX_ALGORITHM_H_
#define TASKS_TASK_3_KOSTIN_A_FOX_ALGORITHM_FOX_ALGORITHM_H_

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <string>

double* SequentialMul(double* matrixa, double* matrixb, int n);

void getRandMatrix(double* matrix, int n);

bool isMatrEqual(double* matrixa, double* matrixb, int n);

void matrMalloc(double** matrix, int n);

void matrCalloc(double** matrix, int n);

double* Fox_algorithm(double* matrixa, double* matrixb, int n);

#endif  // TASKS_TASK_3_KOSTIN_A_FOX_ALGORITHM_FOX_ALGORITHM_H_
