// Copyright 2023 Skotin Alexander
#include "task_2/skotin_a_grid_tor/grid_tor.h"
#include <cmath>

TorusGrid::TorusGrid() {
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
    MPI_Comm_size(MPI_COMM_WORLD, &size_);

    row_ = rank_ / GRID_SIZE;
    col_ = rank_ % GRID_SIZE;

    MPI_Comm_split(MPI_COMM_WORLD, row_, rank_, &row_comm_);
    MPI_Comm_split(MPI_COMM_WORLD, col_, rank_, &col_comm_);

    calculateNeighbors();
}

void TorusGrid::calculateNeighbors() {
    neighbor_north_ = (row_ > 0) ? rank_ - GRID_SIZE : MPI_PROC_NULL;
    neighbor_south_ = (row_ < GRID_SIZE - 1) ? rank_ + GRID_SIZE
                                                 : MPI_PROC_NULL;
    neighbor_east_ = (col_ < GRID_SIZE - 1) ? rank_ + 1 : MPI_PROC_NULL;
    neighbor_west_ = (col_ > 0) ? rank_ - 1 : MPI_PROC_NULL;
}

int TorusGrid::determineNextNode(int source, int destination) const {
    // Измененный алгоритм определения следующего узла
    int row_diff = std::abs(source / GRID_SIZE - destination / GRID_SIZE);
    int col_diff = std::abs(source % GRID_SIZE - destination % GRID_SIZE);

    if (row_diff > col_diff) {
        return (source < destination) ? source + GRID_SIZE : source - GRID_SIZE;
    } else if (col_diff > 0) {
        return (source < destination) ? source + 1 : source - 1;
    }
    return -1;
}

void TorusGrid::sendToNeighbor(void* data, int count, MPI_Datatype datatype,
                               int source, int destination, int tag) {
    int next = determineNextNode(source, destination);
    if (rank_ == source) {
        MPI_Send(data, count, datatype, next, tag, MPI_COMM_WORLD);
    } else if (rank_ == next) {
        MPI_Status status;
        MPI_Recv(data, count, datatype, source, tag, MPI_COMM_WORLD, &status);
        if (next != destination) {
            int next_to_send = determineNextNode(next, destination);
            MPI_Send(data, count, datatype, next_to_send, tag, MPI_COMM_WORLD);
        }
    }
}
