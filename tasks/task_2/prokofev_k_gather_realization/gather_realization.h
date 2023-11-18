// Copyright 2023 Prokofev Kirill
#ifndef TASKS_TASK_2_PROKOFEV_K_GATHER_REALIZATION_H_
#define TASKS_TASK_2_PROKOFEV_K_GATHER_REALIZATION_H_

#include <mpi.h>
#include <iostream>

int Gather(const void* sbuf, int scount, MPI_Datatype stype, void* rbuf, int rcount, MPI_Datatype rtype, int root, MPI_Comm comm);
#endif  // TASKS_TASK_2_PROKOFEV_K_GATHER_REALIZATION_H_
