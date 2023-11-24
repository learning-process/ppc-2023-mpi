// Copyright 2023 Kostin Artem
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <functional>
#include "task_2/kostin_a_circle_topology/circle_topology.h"

int next_rank(int r) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    return (r + 1) % size;
}

int prev_rank(int r) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    return (r - 1 + size) % size;
}

void send_data(int* data, int fromProc, int toProc) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (fromProc == toProc)
        return;
    if (world_rank == fromProc)
        MPI_Send(data, 1, MPI_INT, next_rank(world_rank), 0, MPI_COMM_WORLD);
    else if (world_rank != toProc) {
        MPI_Recv(data, 1, MPI_INT, prev_rank(world_rank), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(data, 1, MPI_INT, next_rank(world_rank), 0, MPI_COMM_WORLD);
    } else if (world_rank == toProc)
        MPI_Recv(data, 1, MPI_INT, prev_rank(world_rank), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}
