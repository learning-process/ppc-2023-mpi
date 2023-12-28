// Copyright 2023 Troitskiy Alexandr

#include <mpi.h>
#include "./lin_topology.h"


bool IsInRoute(int current, int source, int destination) {
    bool forward_direction = destination > source;
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
    int rank = 0, world_size = 0;;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &world_size);

    if (source >= world_size || destination >= world_size) {
        return;
    }
    if (destination == source) {
        return;
    }
    if (!IsInRoute(rank, source, destination)) {
        return;
    }

    bool forward_direction = destination > source;
    if (source == rank) {
        MPI_Send(data, count, datatype, GetNextNode(rank, forward_direction), tag, comm);
    } else if (IsInRoute(rank, GetNextNode(source, forward_direction),
                         GetPreviousNode(destination, forward_direction))) {
        MPI_Recv(data, count, datatype, GetPreviousNode(rank, forward_direction), tag, comm, MPI_STATUS_IGNORE);
        MPI_Send(data, count, datatype, GetNextNode(rank, forward_direction), tag, comm);
    } else {
        MPI_Recv(data, count, datatype, GetPreviousNode(rank, forward_direction), tag, comm, MPI_STATUS_IGNORE);
    }
}
