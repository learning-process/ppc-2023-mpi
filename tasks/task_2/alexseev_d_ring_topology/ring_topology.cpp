// Copyright 2023 Alexseev Danila

#include "task_2/alexseev_d_ring_topology/ring_topology.h"

int getRandomInt(int min, int max) {
    srand(std::time(0));
    int randomNumber = min + (std::rand() % (max - min + 1));
    return randomNumber;
}

bool isRceiverLarger(int sourceRank, int receiverRank) {
    return receiverRank > sourceRank;
}

bool isClockwise(int sourceRank, int receiverRank) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    return (isRceiverLarger(sourceRank, receiverRank)
        && size - receiverRank + sourceRank < receiverRank - sourceRank)
        || (!isRceiverLarger(sourceRank, receiverRank)
        && size - sourceRank + receiverRank < sourceRank - receiverRank);
}

int nextRank(int rank) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    return (rank + 1) % size;
}

int prevRank(int rank) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    return (rank - 1 + size) % size;
}

void sendMessage(int* message, int sourceRank, int receiverRank) {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (sourceRank == receiverRank)
        return;
    if (isClockwise(sourceRank, receiverRank)) {
        if (rank == sourceRank) {
            MPI_Send(message, 1, MPI_INT, nextRank(rank), 0, MPI_COMM_WORLD);
        } else if ((isRceiverLarger(sourceRank, receiverRank) && rank > sourceRank && rank < receiverRank)
            || (!isRceiverLarger(sourceRank, receiverRank) && (rank < receiverRank || rank > sourceRank))) {
            MPI_Recv(message, 1, MPI_INT, prevRank(rank), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(message, 1, MPI_INT, nextRank(rank), 0, MPI_COMM_WORLD);
        } else if (rank == receiverRank) {
            MPI_Recv(message, 1, MPI_INT, prevRank(rank), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    } else {
        if (rank == sourceRank) {
            MPI_Send(message, 1, MPI_INT, prevRank(rank), 0, MPI_COMM_WORLD);
        } else if ((isRceiverLarger(sourceRank, receiverRank) && (rank > receiverRank || rank < sourceRank))
            || (!isRceiverLarger(sourceRank, receiverRank) && rank < sourceRank && rank > receiverRank)) {
            MPI_Recv(message, 1, MPI_INT, nextRank(rank), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(message, 1, MPI_INT, prevRank(rank), 0, MPI_COMM_WORLD);
        } else if (rank == receiverRank) {
            MPI_Recv(message, 1, MPI_INT, nextRank(rank), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
}
