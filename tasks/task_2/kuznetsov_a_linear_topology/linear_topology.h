// Copyright 2023 Kuznetsov Artem
#ifndef TASKS_TASK_2_KUZNETSOV_A_LINEAR_TOPOLOGY_LINEAR_TOPOLOGY_H_
#define TASKS_TASK_2_KUZNETSOV_A_LINEAR_TOPOLOGY_LINEAR_TOPOLOGY_H_

#include <mpi.h>

int get_next(int current, bool router);
int get_prev(int current, bool router);
bool in_route(int current, int src, int dest, bool router);
void send_data_linear(void* data, int count, MPI_Datatype datatype, int src,
                    int dest, int tag, MPI_Comm comm);

#endif  // TASKS_TASK_2_KUZNETSOV_A_LINEAR_TOPOLOGY_LINEAR_TOPOLOGY_H_
