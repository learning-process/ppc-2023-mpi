// Copyright 2023 Ulyanov Daniil

#include "task_2/ulyanov_d_star_topology/star_topology.h"

int send_int(const int& data, int sender, int recipient, int root) {
  int num, rank, local_data = -1;
  MPI_Status status;

  MPI_Comm_size(MPI_COMM_WORLD, &num);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (sender > num || recipient > num || root > num) {
    return data;
  }

  if (sender == recipient) {
    return data;
  }

  if (rank == root) {
    if (root != sender) {
      MPI_Recv(&local_data, 1, MPI_INT, sender, 0, MPI_COMM_WORLD, &status);
    } else {
      local_data = data;
    }
    if (root != recipient) {
      MPI_Send(&local_data, 1, MPI_INT, recipient, 0, MPI_COMM_WORLD);
    }
  } else if (rank == sender) {
    MPI_Send(&data, 1, MPI_INT, root, 0, MPI_COMM_WORLD);
    local_data = data;
  } else if (rank == recipient) {
    MPI_Recv(&local_data, 1, MPI_INT, root, 0, MPI_COMM_WORLD, &status);
  }

  return local_data;
}
