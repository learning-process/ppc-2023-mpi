// Copyright 2023 Khramov Ivan
#include <mpi.h>
#include "task_2/khramov_i_star_topology/star_topology.h"


void sendDataStar(
    MPI_Comm world,
    int master,
    int source,
    int receiver,
    int tag,
    void* data,
    MPI_Datatype datatype) {
    int world_rank, world_size;

    MPI_Comm_size(world, &world_size);
    MPI_Comm_rank(world, &world_rank);


    if (!(source < world_size && receiver < world_size)) return;
    if (receiver - source == 0) return;

    if (world_rank == receiver && source == master) {
        MPI_Recv(data, 1, datatype, master, tag, world, MPI_STATUS_IGNORE);
        return;
    }

    if (world_rank == source) {
        if (source == master) {
            MPI_Send(data, 1, datatype, receiver, tag, world);
            return;
        } else {
            MPI_Send(data, 1, datatype, master, tag, world);
        }
    } else if (world_rank == master) {
        if (master != receiver) {
            MPI_Recv(data, 1, datatype, source, tag, world, MPI_STATUS_IGNORE);
            MPI_Send(data, 1, datatype, receiver, tag, world);
        } else {
            MPI_Recv(data, 1, datatype, source, tag, world, MPI_STATUS_IGNORE);
        }
    } else if (world_rank == receiver) {
        MPI_Recv(data, 1, datatype, master, tag, world, MPI_STATUS_IGNORE);
    }
}
