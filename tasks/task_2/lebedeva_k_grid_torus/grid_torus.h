// Copyright 2023 Lebedeva Ksenia

#ifndef TASKS_TASK_2_LEBEDEVA_K_GRID_TORUS_GRID_TORUS_H_
#define TASKS_TASK_2_LEBEDEVA_K_GRID_TORUS_GRID_TORUS_H_

#include <mpi.h>

int getNext(int sender, int receiver);

void sendData(void* data, int count, MPI_Datatype datatype,
              int sender, int receiver, int tag, MPI_Comm comm);

void sendDataInternal(void* data, int count, MPI_Datatype datatype,
              int sender, int receiver, int final, int tag, MPI_Comm comm);

#endif  // TASKS_TASK_2_LEBEDEVA_K_GRID_TORUS_GRID_TORUS_H_
