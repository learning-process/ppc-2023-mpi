// Copyright 2023 Chuvashov Andrey

#include "../tasks/task_2/chuvashov_a_ring_topology/ring_topology.h"

void SendDataByRing(void* data, int count, MPI_Datatype datatype,
    int source, int destination, int tag, MPI_Comm communicator) {
    int rank, size;
    MPI_Comm_rank(communicator, &rank);
    MPI_Comm_size(communicator, &size);


}
