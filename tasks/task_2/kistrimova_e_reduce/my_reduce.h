// Copyright 2023 Kistrimova Ekaterina
#ifndef TASKS_TASK_2_KISTRIMOVA_E_REDUCE_MY_REDUCE_H_
#define TASKS_TASK_2_KISTRIMOVA_E_REDUCE_MY_REDUCE_H_

#include <mpi.h>

struct proc_tree_props {
  int parent = -1;
  int children[2] = { -1, -1 };
};

void calculate_tree_props(int proc_rank, int proc_num,
  proc_tree_props* props, int* path = NULL, int* path_size = NULL);

int my_mpi_reduce(const void* sendbuf, void* recvbuf,
  int count, MPI_Datatype type, MPI_Op op, int root, MPI_Comm comm);

#endif  // TASKS_TASK_2_KISTRIMOVA_E_REDUCE_MY_REDUCE_H_
