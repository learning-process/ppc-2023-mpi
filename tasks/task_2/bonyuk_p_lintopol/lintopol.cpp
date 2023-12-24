  // Copyright 2023 Bonyuk Petr

#include <mpi.h>
#include "task_2/bonyuk_p_lintopol/lintopol.h"

bool is_in_path(int current, int src, int dest, bool is_forward) {
    if (is_forward) {
        return (current >= src && current <= dest);
    } else {
        return (current <= src && current >= dest);
    }
}

int next_node(int cur, bool rout) {
    return rout ? cur + 1 : cur - 1;
}

int prev_node(int cur, bool rout) {
    return rout ? cur - 1 : cur + 1;
}

void Sends_data_lin_acr_the_topol(void* data, int count, MPI_Datatype datatype, int src,
    int destination, int tag, MPI_Comm comm) {
    int rank;
    int size;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (src >= size || destination >= size) return;
    if (src == destination) return;

    bool is_forward = (destination > src);
    bool is_in_route = is_in_path(rank, src, destination, is_forward);

    if (!is_in_route)
        return;

    if (rank == src) {
        MPI_Send(data, count, datatype, next_node(rank, is_forward), tag, comm);
    } else if (is_in_path(rank, next_node(src, is_forward), prev_node(destination, is_forward), is_forward)) {
        MPI_Recv(data, count, datatype, prev_node(rank, is_forward), tag, comm, MPI_STATUS_IGNORE);
        MPI_Send(data, count, datatype, next_node(rank, is_forward), tag, comm);
    } else {
        MPI_Recv(data, count, datatype, prev_node(rank, is_forward), tag, comm, MPI_STATUS_IGNORE);
    }
}
