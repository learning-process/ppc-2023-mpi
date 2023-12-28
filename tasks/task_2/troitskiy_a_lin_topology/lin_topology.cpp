// Copyright 2023 Troitskiy Alexandr

#include <mpi.h>
#include "task_2/troitskiy_a_lin_topology/lin_topology.h"


bool IsInRoute(int current, int source, int destination, bool forward_direction) {
    return !forward_direction && destination <= current && current <= source ||
           forward_direction && source <= current && current <= destination;
}

int GetPreviousNode(int current, bool forward_direction) {
    return forward_direction ? current - 1 : current + 1;
}

int GetNextNode(int current, bool forward_direction) {
    return forward_direction ? current + 1 : current - 1;
}

void SendDataLinearTopology(void *data, int count, MPI_Datatype datatype, int source,
                            int destination, int tag, MPI_Comm comm) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (!(source < size && destination < size)) return;
    if (destination - source == 0) return;

    bool is_forward = (destination - source > 0);

    if (!IsInRoute(rank, source, destination, is_forward)) return;

    if (rank == source) {
        MPI_Send(data, count, datatype, GetNextNode(rank, is_forward), tag, comm);
    } else if (IsInRoute(rank, GetNextNode(source, is_forward), GetPreviousNode(destination, is_forward), is_forward)) {
        MPI_Recv(data, count, datatype, GetPreviousNode(rank, is_forward), tag, comm, MPI_STATUS_IGNORE);
        MPI_Send(data, count, datatype, GetNextNode(rank, is_forward), tag, comm);
    } else {
        MPI_Recv(data, count, datatype, GetPreviousNode(rank, is_forward), tag, comm, MPI_STATUS_IGNORE);
    }
}
