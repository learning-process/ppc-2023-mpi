// Copyright 2023 Morgachev Stepan
#ifndef TASKS_TASK_2_MORGACHEV_S_HYPERCUBE_HYPERCUBE_H_
#define TASKS_TASK_2_MORGACHEV_S_HYPERCUBE_HYPERCUBE_H_

#include <mpi.h>
#include <vector>
#include <random>

int calculateNextVertex(int sender, int dest);
void HypercubeSend(void* data,
    int length,
    MPI_Datatype datatype,
    int sender,
    int dest,
    int tag,
    MPI_Comm comm);
void sendDataInHyperCube(void* buffer,
    int count,
    MPI_Datatype datatype,
    int sender,
    int dest,
    int finalDest,
    int tag,
    MPI_Comm comm);

#endif  // TASKS_TASK_2_MORGACHEV_S_HYPERCUBE_HYPERCUBE_H_
