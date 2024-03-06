// Copyright 2023 Fedotov Kirill
#ifndef TASKS_TASK_2_FEDOTOV_K_GATHER_GATHER_H_
#define TASKS_TASK_2_FEDOTOV_K_GATHER_GATHER_H_

#include <mpi.h>
#include <cstring>
#include <cstdlib>
#include <iostream>

int Gather(const void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int recvcount,
    MPI_Datatype recvtype, int root, MPI_Comm comm);

#endif  // TASKS_TASK_2_FEDOTOV_K_GATHER_GATHER_H_
