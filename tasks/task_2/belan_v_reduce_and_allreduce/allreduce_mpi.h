/* Copyright 2023 Belan Vadim */

#ifndef TASKS_TASK_2_BELAN_V_REDUCE_AND_ALLREDUCE_ALLREDUCE_MPI_H_
#define TASKS_TASK_2_BELAN_V_REDUCE_AND_ALLREDUCE_ALLREDUCE_MPI_H_

#include <mpi.h>

double calculate_average();
void mpi_reduce(int local_value, int* global_sum, int root);
void mpi_all_reduce(int local_value, int* inclusive_sum);

#endif  // TASKS_TASK_2_BELAN_V_REDUCE_AND_ALLREDUCE_ALLREDUCE_MPI_H_
