//  Copyright 2023 Ryabkov Vladislav


#ifndef TASKS_TASK_1_RYABKOV_V_NUM_OF_ALTERNATIONS_SIGNS_ALTER_SIGN_H_
#define TASKS_TASK_1_RYABKOV_V_NUM_OF_ALTERNATIONS_SIGNS_ALTER_SIGN_H_

#include <mpi.h>
#include <vector>
#include <random>


void RandVec(int* V, int n);
int SerialSum(const int* V, int n);
int ParallelSum(const int* V, int n);


#endif  // TASKS_TASK_1_RYABKOV_V_NUM_OF_ALTERNATIONS_SIGNS_ALTER_SIGN_H_
