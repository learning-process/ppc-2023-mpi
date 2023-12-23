// Copyright 2023 Sokolova Daria
#ifndef TASKS_TASK_2_SOKOLOVA_D_SCATTER_SCATTER_H_
#define TASKS_TASK_2_SOKOLOVA_D_SCATTER_SCATTER_H_

#include <mpi.h>
void scatter(void* sendbuf, int sendcount, MPI_Datatype sendtype,
    void* recvbuf, int recvcount, MPI_Datatype recvtype,
    int root, MPI_Comm comm);

#endif  // TASKS_TASK_2_SOKOLOVA_D_SCATTER_SCATTER_H_
