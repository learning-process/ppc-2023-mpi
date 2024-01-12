// Copyright 2023 Safronov Mikhail
#pragma once
#ifndef TASKS_TASK_3_SAFRONOV_DIJKSTRA_DIJKSTRA_H_
#define TASKS_TASK_3_SAFRONOV_DIJKSTRA_DIJKSTRA_H_

#include <mpi.h>
#include <limits.h>
#include <algorithm>
#include <iostream>

extern MPI_Comm test_comm;

int minDistance(int dist[], bool sptSet[]);
void dijkstra(MPI_Comm comm, int** graph, int n, int src, int* resDist);

#endif  // TASKS_TASK_3_SAFRONOV_DIJKSTRA_DIJKSTRA_H_
