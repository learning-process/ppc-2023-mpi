// Copyright 2023 Vetoshnikova Ekaterina


#ifndef TASKS_TASK_2_VETOSHNIKOVA_K_BUBBLE_SORT_BUBBLE_SORT_H_
#define TASKS_TASK_2_VETOSHNIKOVA_K_BUBBLE_SORT_BUBBLE_SORT_H_

#include <mpi.h>
#include <vector>
#include <utility>

int* array_generation(int mn, int mx, int sz);
void ParallelBubbleSort(int* global_arr, int count_size_arr);
void SequentialBubbleSort(int* arr, int count_size_arr);

#endif  // TASKS_TASK_2_VETOSHNIKOVA_K_BUBBLE_SORT_BUBBLE_SORT_H_
