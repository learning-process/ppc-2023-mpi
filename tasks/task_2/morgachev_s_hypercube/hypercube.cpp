// Copyright 2023 Morgachev Stepan
#include "task_2/morgachev_s_hypercube/hypercube.h"

int calculateNextVertex(int sender, int dest) {
    if (sender == dest) {
        return sender;
    }

    int mask = 1;

    while (true) {
        if ((sender & mask) == (dest & mask)) {
            mask = mask << 1;
            continue;
        }

        return sender ^ mask;
    }
}

void HypercubeSend(void* data,
    int length,
    MPI_Datatype datatype,
    int from,
    int dest,
    int tag,
    MPI_Comm comm) {
    int next = calculateNextVertex(from, dest);
    sendDataInHyperCube(data, length, datatype, from, next, dest, tag, comm);
}

void sendDataInHyperCube(void* buffer,
    int count,
    MPI_Datatype datatype,
    int sender,
    int dest,
    int finalDest,
    int tag,
    MPI_Comm comm) {
    int rank;
    MPI_Comm_rank(comm, &rank);

    if (rank == sender) {
        MPI_Send(buffer, count, datatype, dest, tag, comm);
    }

    if (rank == dest) {
        MPI_Status status;
        MPI_Recv(buffer, count, datatype, sender, tag, comm, &status);
    }

    if (dest == finalDest) {
        return;
    }

    int next = calculateNextVertex(dest, finalDest);
    sendDataInHyperCube(buffer,
        count,
        datatype,
        dest,
        next,
        finalDest,
        tag,
        comm);
}
