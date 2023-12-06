// Copyright 2023 Chuvashov Andrey

#ifndef TASKS_TASK_2_CHUVASHOV_A_RING_TOPOLOGY_RING_TOPOLOGY_H_
#define TASKS_TASK_2_CHUVASHOV_A_RING_TOPOLOGY_RING_TOPOLOGY_H_

#include <mpi.h>
#include <iostream>
#include <random>

int nextNode(int currentNode, int size);
int previousNode(int currentNode, int size);
bool checker(int currentNode, int source, int destination, int size);
void SendDataByRing(void* data, int count, MPI_Datatype datatype,
    int source, int destination, int tag, MPI_Comm communicator);

#endif  // TASKS_TASK_2_CHUVASHOV_A_RING_TOPOLOGY_RING_TOPOLOGY_H_
