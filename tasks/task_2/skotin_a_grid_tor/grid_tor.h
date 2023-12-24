// Copyright 2023 Skotin Alexander
#ifndef TASKS_TASK_2_SKOTIN_A_GRID_TOR_GRID_TOR_H_
#define TASKS_TASK_2_SKOTIN_A_GRID_TOR_GRID_TOR_H_

#include <mpi.h>
#include <vector>
#include <utility>

std::pair<int, int> getGridDimensions(int totalProcs);
int getNextNode(int current, int destination, int rows, int cols);
void sendData(int source, int destination, const std::vector<int>& data,
                                                         MPI_Comm comm);

#endif  // TASKS_TASK_2_SKOTIN_A_GRID_TOR_GRID_TOR_H_
