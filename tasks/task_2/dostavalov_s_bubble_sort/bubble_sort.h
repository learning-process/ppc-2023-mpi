// Copyright 2023 Dostavalov Semen

#ifndef TASKS_TASK_2_DOSTAVALOV_S_BUBBLE_SORT_BUBBLE_SORT_H_
#define TASKS_TASK_2_DOSTAVALOV_S_BUBBLE_SORT_BUBBLE_SORT_H_

#include <mpi.h>
#include <vector>
#include <string>

void typical_bubble_s(int* myarray, int mysize);
int generateNewArr(int al);
int* doParallel_BubbleS(int* ourarr, int arr_size_in);
int* bubble_s_with_return(int* arr, int n);

#endif  // TASKS_TASK_2_DOSTAVALOV_S_BUBBLE_SORT_BUBBLE_SORT_H_
