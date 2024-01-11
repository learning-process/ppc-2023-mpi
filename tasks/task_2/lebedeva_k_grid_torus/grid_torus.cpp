// Copyright 2023 Lebedeva Ksenia

#include <algorithm>
#include "./grid_torus.h"
#include "task_2/lebedeva_k_grid_torus/grid_torus.h"

int getNext(int sender, int receiver) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (sender == receiver) {
        return sender;
    }
    int row_delta = abs(sender / size - receiver / size);
    int column_delta = abs(sender % size - receiver % size);
    if (row_delta > 0) {
        return sender + (sender < receiver ? size : -size);
    } else if (column_delta > 0) {
        return sender + (sender < receiver ? 1 : -1);
    } else {
        return -1;
    }
}

void sendData(void* data, int count, MPI_Datatype datatype,
              int sender, int receiver, int tag, MPI_Comm comm) {
    int next = getNext(sender, receiver);
    sendDataInternal(data, count, datatype, sender, next, receiver, tag, comm);
}

void sendDataInternal(void* data, int count, MPI_Datatype datatype,
                      int sender, int receiver, int final, int tag, MPI_Comm comm) {
    int size, rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    if (sender == receiver) {
        return;
    }

    if (rank == sender) {
        MPI_Send(data, count, datatype, receiver, tag, comm);
    }

    if (rank == receiver) {
        MPI_Status status;
        MPI_Recv(data, count, datatype, sender, tag, comm, &status);
    }

    int next = getNext(receiver, final);
    sendDataInternal(data, count, datatype, receiver, next, final, tag, comm);
}
