// Copyright 2023 Skotin Alexander
#include "task_2/skotin_a_grid_tor/grid_tor.h"
#include <cmath>

std::pair<int, int> getGridDimensions(int totalProcs) {
    int rows = static_cast<int>(std::sqrt(totalProcs));
    int cols = totalProcs / rows;
    return {rows, cols};
}

int getNextNode(int current, int destination, int rows, int cols) {
    int currentRow = current / cols;
    int currentCol = current % cols;
    int destRow = destination / cols;
    int destCol = destination % cols;

    int nextRow = currentRow;
    int nextCol = currentCol;

    if (currentRow != destRow) {
        nextRow = (currentRow + 1) % rows;
    } else if (currentCol != destCol) {
        nextCol = (currentCol + 1) % cols;
    }

    return nextRow * cols + nextCol;
}

void sendData(int source, int destination, int tag, MPI_Comm comm,
    const std::vector<int>& data) {
    int rank;
    MPI_Comm_rank(comm, &rank);

    if (rank == source) {
        MPI_Send(data.data(), data.size(), MPI_INT, destination, tag, comm);
    } else if (rank == destination) {
        std::vector<int> receivedData(data.size());
        MPI_Recv(receivedData.data(), receivedData.size(), MPI_INT, source,
            tag, comm, MPI_STATUS_IGNORE);
    }
}
