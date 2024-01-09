// Copyright 2023 Loginov Maxim

#ifndef TASKS_TASK_2_LOGINOV_M_GATHER_GATHER_H_
#define TASKS_TASK_2_LOGINOV_M_GATHER_GATHER_H_

#include <mpi.h>
#include <cstring>
#include <cstdlib>

int Gather(const void* sbuf, int scount,
   MPI_Datatype stype, void* rbuf, int rcount,
   MPI_Datatype rtype, int root, MPI_Comm comm);
#endif  // TASKS_TASK_2_LOGINOV_M_GATHER_GATHER_H_
