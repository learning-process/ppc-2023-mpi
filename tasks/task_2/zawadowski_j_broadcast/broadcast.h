// Copyright 2023 Zawadowski Jan
#ifndef TASKS_TASK_2_ZAWADOWSKI_J_BROADCAST_BROADCAST_H_
#define TASKS_TASK_2_ZAWADOWSKI_J_BROADCAST_BROADCAST_H_

#include <mpi.h>
#include <vector>
#include <random>

void broadcast(void* buffer, int count, MPI_Datatype datatype,
               int root, MPI_Comm comm);

#endif  // TASKS_TASK_2_ZAWADOWSKI_J_BROADCAST_BROADCAST_H_
