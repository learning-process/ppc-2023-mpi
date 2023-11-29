// Copyright 2023 Videneva Ekaterina
#include "task_2/videneva_e_ring/ring.h"

int generateRandomNumbers(int min, int max) {
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

void sendMessage(int* message, int sourceRank, int receiverRank) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int nextRank;
    int prevRank;
    nextRank=(rank + 1)%size;
    prevRank=(rank - 1+size)%size;
    if (rank == 0)
       prevRank = size - 1;
    if (rank == size - 1)
        nextRank = 0;

    if (sourceRank == receiverRank)
        return;
    if (isClockwise(sourceRank, receiverRank)) {
        if (rank == sourceRank) {
            MPI_Send(message, 1, MPI_INT, nextRank, 0, MPI_COMM_WORLD);
        } else if ((isRceiverLarger(sourceRank, receiverRank) && rank > sourceRank && rank < receiverRank)
            || (!isRceiverLarger(sourceRank, receiverRank) && (rank < receiverRank || rank > sourceRank))) {
            MPI_Recv(message, 1, MPI_INT, prevRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(message, 1, MPI_INT, nextRank, 0, MPI_COMM_WORLD);
        } else if (rank == receiverRank) {
            MPI_Recv(message, 1, MPI_INT, prevRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    } else {
        if (rank == sourceRank) {
            MPI_Send(message, 1, MPI_INT, prevRank, 0, MPI_COMM_WORLD);
        } else if ((isRceiverLarger(sourceRank, receiverRank) && (rank > receiverRank || rank < sourceRank))
            || (!isRceiverLarger(sourceRank, receiverRank) && rank < sourceRank && rank > receiverRank)) {
            MPI_Recv(message, 1, MPI_INT, nextRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(message, 1, MPI_INT, prevRank, 0, MPI_COMM_WORLD);
        } else if (rank == receiverRank) {
            MPI_Recv(message, 1, MPI_INT, nextRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
}