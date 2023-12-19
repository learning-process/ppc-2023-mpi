// Copyright 2023 Moiseev Nikita
#include <mpi.h>
#include "task_2/moiseev_n_linear_topology/linear_topology.h"

int getForwardRank(int currentRank, bool isForward) {
    return isForward ? currentRank + 1 : currentRank - 1;
}

int getBackwardRank(int currentRank, bool isForward) {
    return isForward ? currentRank - 1 : currentRank + 1;
}

bool isInRoute(int currentRank, int sourceRank, int destRank, bool isForward) {
    return isForward && currentRank >= sourceRank && currentRank <= destRank ||
        !isForward && currentRank <= sourceRank && currentRank >= destRank;
}


void sendDataLinear(void* data, int count, MPI_Datatype datatype,
    int sourceRank, int destRank, int tag, MPI_Comm comm) {
    int rank, sizeWorld;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &sizeWorld);
    if (sourceRank >= sizeWorld || destRank >= sizeWorld) return;
    if (destRank - sourceRank == 0) return;
    bool isForward = (destRank - sourceRank > 0);
    if (!(isForward ? (rank >= sourceRank && rank <= destRank) :
        (rank <= sourceRank && rank >= destRank))) return;
    if (rank == sourceRank) {
        MPI_Send(data, count, datatype, (isForward ? rank + 1 : rank - 1), tag, comm);
    } else {
        int prevRank = (isForward ? rank - 1 : rank + 1);
        int nextRank = (isForward ? rank + 1 : rank - 1);
        MPI_Recv(data, count, datatype, prevRank, tag, comm, MPI_STATUS_IGNORE);
        if (isInRoute(rank, getForwardRank(sourceRank, isForward), getBackwardRank(destRank, isForward), isForward)) {
            MPI_Send(data, count, datatype, nextRank, tag, comm);
        }
    }
}
