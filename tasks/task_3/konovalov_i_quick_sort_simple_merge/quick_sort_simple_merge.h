// Copyright 2023 Konovalov Igor
#ifndef TASKS_TASK_3_KONOVALOV_I_QUICK_SORT_SIMPLE_MERGE_QUICK_SORT_SIMPLE_MERGE_H_
#define TASKS_TASK_3_KONOVALOV_I_QUICK_SORT_SIMPLE_MERGE_QUICK_SORT_SIMPLE_MERGE_H_

#include <mpi.h>
#include <vector>
#include <iostream>
#include <random>
#include <time.h>

void swap(int* arr, int i, int j);
void quickSortRec(int n, int* arr)
void merge(int n1, int n2, int* left, int* right, int* res);
void quickSortPar(int* res, size_t n);
bool check(int n, int* arr);

#endif  // TASKS_TASK_3_KONOVALOV_I_QUICK_SORT_SIMPLE_MERGE_QUICK_SORT_SIMPLE_MERGE_H_
