// Copyright 2023 Simonyan Suren
#ifndef TASKS_TASK_3_SIMONYAN_S_QUICKSORT_QUICKSORT_H_
#define TASKS_TASK_3_SIMONYAN_S_QUICKSORT_QUICKSORT_H_

#include <mpi.h>
#include <iostream>

void quickSortFunction(double* array, int left, int right);
double* Sort(double* arr, int n);
double* merge(double* arr1, int n1, double* arr2, int n2);
int getothersize(int n, int step, int r);
#endif  // TASKS_TASK_3_SIMONYAN_S_QUICKSORT_QUICKSORT_H_
