// Copyright 2023 Kuznetsov Artem
#include "task_2/kuznetsov_a_linear_topology/linear_topology.h"

int get_next(int current, bool router) {
  return router ? current + 1 : current - 1;
}

int get_prev(int current, bool router) {
  return router ? current - 1 : current + 1;
}

bool in_route(int current, int src, int dest, bool router) {
  return router && current >= src && current <= dest ||
         !router && current <= src && current >= dest;
}

void send_data_linear(void* data, int count, MPI_Datatype datatype, int src,
                      int dest, int tag, MPI_Comm comm) {
  int rank = 0;
  int sizeWorld = 0;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &sizeWorld);

  if (!(src < sizeWorld && dest < sizeWorld)) return;
  if (dest - src == 0) return;

  bool router = (dest - src > 0);

  if (!in_route(rank, src, dest, router)) return;

  if (rank == src) {
    MPI_Send(data, count, datatype, get_next(rank, router), tag, comm);
  } else if (in_route(rank, get_next(src, router), get_prev(dest, router),
                      router)) {
    MPI_Recv(data, count, datatype, get_prev(rank, router), tag, comm,
             MPI_STATUS_IGNORE);
    MPI_Send(data, count, datatype, get_next(rank, router), tag, comm);
  } else {
    MPI_Recv(data, count, datatype, get_prev(rank, router), tag, comm,
             MPI_STATUS_IGNORE);
  }
}
