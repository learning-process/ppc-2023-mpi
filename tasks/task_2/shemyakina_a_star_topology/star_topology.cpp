// Copyright 2023 Shemyakina Alesya
#include <mpi.h>
#include "task_2/shemyakina_a_star_topology/star_topology.h"

void StarData(MPI_Comm world, int master, int source, int receiver, int tag, void* data, MPI_Datatype datatype) {
    int size, rank;

    MPI_Comm_size(world, &size);
    MPI_Comm_rank(world, &rank);

    if (!(source < size && receiver < size)) return;
    if (receiver - source == 0) return;

    if (rank == receiver && source == master) {
        MPI_Recv(data, 1, datatype, master, tag, world, MPI_STATUS_IGNORE);
        return;
    }

    if (rank == source) {
        if (source == master) {
            MPI_Send(data, 1, datatype, receiver, tag, world);
            return;
        } else {
            MPI_Send(data, 1, datatype, master, tag, world);
        }
    } else if (rank == master) {
        if (master != receiver) {
            MPI_Recv(data, 1, datatype, source, tag, world, MPI_STATUS_IGNORE);
            MPI_Send(data, 1, datatype, receiver, tag, world);
        } else {
            MPI_Recv(data, 1, datatype, source, tag, world, MPI_STATUS_IGNORE);
        }
    } else if (rank == receiver) {
        MPI_Recv(data, 1, datatype, master, tag, world, MPI_STATUS_IGNORE);
    }
}
