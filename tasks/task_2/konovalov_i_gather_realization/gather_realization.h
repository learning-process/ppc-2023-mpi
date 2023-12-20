// Copyright 2023 Konovalov Igor
#ifndef TASKS_TASK_2_KONOVALOV_I_GATHER_REALIZATION_GATHER_REALIZATION_H_
#define TASKS_TASK_2_KONOVALOV_I_GATHER_REALIZATION_GATHER_REALIZATION_H_

#include <mpi.h>
#include <cstdlib>

int custom_gather(const void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
#endif  // TASKS_TASK_2_KONOVALOV_I_GATHER_REALIZATION_GATHER_REALIZATION_H_