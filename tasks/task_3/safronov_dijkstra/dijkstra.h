// Copyright 2023 Safronov Mikhail
#ifndef TASKS_TASK_3_SAFRONOV_DIJKSTRA_DIJKSTRA_H_
#define TASKS_TASK_3_SAFRONOV_DIJKSTRA_DIJKSTRA_H_

#include <mpi.h>
#include <climits>
#include <iostream>

int minDistance(int dist[], bool sptSet[]);
void printSolution(int dist[], int n);
void dijkstra(int** graph, int n, int src, int* resDist);

#endif  // TASKS_TASK_3_SAFRONOV_DIJKSTRA_DIJKSTRA_H_
