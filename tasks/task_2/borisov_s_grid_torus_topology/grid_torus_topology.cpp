// Copyright 2023 Borisov Saveliy
#include "task_2/borisov_s_grid_torus_topology/grid_torus_topology.h"


int getNext(int rows, int cols, int rank, int rankDestination) {
    if (rank == rankDestination) {
        return rankDestination;
    }
    int numVertices = rows * cols;

    int horizontalDistance = rankDestination % rows - rank % rows;
    int verticalDistance = rankDestination / rows - rank / rows;
    
    if (horizontalDistance != 0) {
        int sign = (horizontalDistance > 0) ? 1 : ((horizontalDistance < 0) ? -1 : 0);
        if (horizontalDistance < sign * cols - horizontalDistance) {
            int right = (rank + 1) % cols == 0 ? rank - (cols - 1) : rank + 1;
            return right;
        } else {
            int left = rank % cols == 0 ? rank + (cols - 1) : rank - 1;
            return left;
        }
    } else if (verticalDistance != 0) {
        int sign = (verticalDistance > 0) ? 1 : ((verticalDistance < 0) ? -1 : 0);
        if (verticalDistance < sign * rows - verticalDistance) {
            int down = (rank + cols) >= numVertices ? rank - (numVertices - cols) : rank + cols;
            return down;
        } else {
            int up = (rank - cols) < 0 ? rank + (numVertices - cols) : rank - cols;
            return up;
        }
    } else {
        return -1;
    }
}

bool calculateGridSize(int numProcesses, int& rows, int& cols) {
    int gridSize = static_cast<int>(std::sqrt(numProcesses));
    while (gridSize > 1) {
        if (numProcesses % gridSize == 0) {
            rows = gridSize;
            cols = numProcesses / gridSize;
            return true;
        }
        gridSize--;
    }
    return false;
}

std::pair<std::vector<int>, std::vector<int>> getPath(int rows, int cols, int senderRank, int receiverRank) {
    std::vector<int> path; 
    std::vector<int> transitions(rows * cols, -1);
    std::vector<int> expectations(rows * cols, -1);
    
    path.push_back(senderRank);
    int nextRank = senderRank;
    while (nextRank != receiverRank) {
        nextRank = getNext(rows, cols, nextRank, receiverRank);
        path.push_back(nextRank);
    }
    
    for (int i = 0; i < path.size(); i++) {
        if (i != path.size() - 1) {
            expectations[path[i]] = path[i + 1];
        }
        if (i != 0) {
            transitions[path[i]] = path[i - 1];
        }
    }
    return std::make_pair(transitions, expectations);
}

void sendDataUsingGridTorus(void* data, int count, MPI_Datatype datatype, int senderRank, int receiverRank, int rows, int cols, int tag, MPI_Comm comm) {
    int process_num;
    MPI_Comm_size(comm, &process_num);

    int rank;
    MPI_Comm_rank(comm, &rank);

    std::pair<std::vector<int>, std::vector<int>> shortPath = getPath(rows, cols, senderRank, receiverRank);
    std::vector<int> senders = shortPath.first;
    std::vector<int> requesters = shortPath.second;
    
    if (rank == senderRank) {
        MPI_Send(data, count, datatype, requesters[rank], tag, comm);
    } else if (rank == receiverRank) {
        MPI_Status status;
        MPI_Recv(data, count, datatype, senders[rank], tag, comm, &status);
    } else {
        if (senders[rank] != -1) {
            MPI_Status status;
            MPI_Recv(data, count, datatype, senders[rank], tag, comm, &status);
            MPI_Send(data, count, datatype, requesters[rank], tag, comm);
        }
    }
}

