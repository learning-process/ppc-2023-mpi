// Copyright 2023 Borovkov Sergey
#ifndef TASKS_TASK_2_BOROVKOV_S_GRID_TOR_GRID_ALG_H
#define TASKS_TASK_2_BOROVKOV_S_GRID_TOR_GRID_ALG_H
#include <mpi.h>

const int N = 5;

struct Mesh {
  int rank, size;
  int row, column;
  MPI_Comm rowComm, colComm;
  int north, south, east, west;
};

Mesh createMesh();
void MeshSend(void *data, int length, MPI_Datatype datatype, int from, int dest,
              int tag, Mesh *mesh);
void meshSendInternal(void *data, int length, MPI_Datatype datatype, int from,
                      int dest, int final, int tag, Mesh *mesh);
int getNext(int from, int dest);
#endif  // TASKS_TASK_2_BOROVKOV_S_GRID_TOR_GRID_ALG_H