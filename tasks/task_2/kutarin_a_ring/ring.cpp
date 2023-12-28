// Copyright 2023 Kutarin Aleksandr
#include "task_2/kutarin_a_ring/ring.h"


int nextRank(int rank) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    return (rank + 1)%size;
}

int prevRank(int rank) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    return (rank - 1+size)%size;
}

bool isDownTime(int rank, int source, int receiver) {
    if (source > receiver)
        if (rank < source && rank > receiver)
            return true;
    if (source < receiver)
        if (rank < source || rank > receiver)
            return true;
    return false;
}

void sendMessage(int* message, int source, int receiver) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (source == receiver)
         return;
    if (rank == receiver) {
         MPI_Recv(message, 1, MPI_INT, prevRank(rank),
          0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if (rank == source) {
         MPI_Send(message, 1, MPI_INT, nextRank(rank),
          0, MPI_COMM_WORLD);
    } else if (rank != receiver && !isDownTime(rank, source, receiver)) {
        MPI_Recv(message, 1, MPI_INT, prevRank(rank),
         0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(message, 1, MPI_INT, nextRank(rank),
         0, MPI_COMM_WORLD);
    }
}
