// Copyright 2023 Skotin Alexander
#include "task_2/skotin_a_grid_tor/grid_tor.h"
#include <cmath>
#include <iostream>

std::pair<int, int> getGridDimensions(int totalProcs) {
    int rows = static_cast<int>(std::sqrt(totalProcs));
    int cols = totalProcs / rows;
    return { rows, cols };
}

int getNextNode(int current, int destination, int rows, int cols) {
    int currentRow = current / cols;
    int currentCol = current % cols;
    int destRow = destination / cols;
    int destCol = destination % cols;

    int rowDist = destRow - currentRow;
    int colDist = destCol - currentCol;

    if (std::abs(rowDist) > rows / 2) {
        if (rowDist > 0) {
            rowDist -= rows;
        } else {
            rowDist += rows;
        }
    }

    if (std::abs(colDist) > cols / 2) {
        if (colDist > 0) {
            colDist -= cols;
        } else {
            colDist += cols;
        }
    }

    int nextRow = currentRow + (rowDist != 0 ? (rowDist > 0 ? 1 : -1) : 0);
    int nextCol = currentCol + (colDist != 0 ? (colDist > 0 ? 1 : -1) : 0);

    nextRow = (nextRow + rows) % rows;
    nextCol = (nextCol + cols) % cols;

    return nextRow * cols + nextCol;
}


void sendData(int source, int destination, const std::vector<int>& data,
                                                        MPI_Comm comm) {
    int rank;
    MPI_Comm_rank(comm, &rank);

    if (rank == source) {
        MPI_Send(data.data(), data.size(), MPI_INT, destination, 0, comm);
    } else if (rank == destination) {
        std::vector<int> receivedData(data.size());
        MPI_Recv(receivedData.data(), receivedData.size(), MPI_INT, source,
                                                0, comm, MPI_STATUS_IGNORE);
    }
}
