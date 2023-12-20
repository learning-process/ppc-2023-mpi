// Copyright 2023 Borisov Saveliy
#ifndef TASKS_TASK_2_BORISOV_S_GRID_TORUS_TOPOLOGY_GRID_TORUS_TOPOLOGY_H_
#define TASKS_TASK_2_BORISOV_S_GRID_TORUS_TOPOLOGY_GRID_TORUS_TOPOLOGY_H_

#include <mpi.h>
#include <cmath>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <iostream>

int getNext(int rows, int cols, int rank, int rankDestination);
bool calculateGridSize(int numProcesses, int& rows, int& cols);
std::pair<std::vector<int>, std::vector<int>> getPath(int rows, int cols, int senderRank, int receiverRank);
void sendDataUsingGridTorus(void* data, int count, MPI_Datatype datatype, int senderRank, int receiverRank, int rows, int cols, int tag, MPI_Comm comm);


#endif  // TASKS_TASK_2_BORISOV_S_GRID_TORUS_TOPOLOGY_GRID_TORUS_TOPOLOGY_H_
