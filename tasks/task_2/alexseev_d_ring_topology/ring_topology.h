// Copyright 2023 Alexseev Danila

#ifndef TASKS_TASK_2_ALEXSEEV_D_RING_TOPOLOGY_RING_TOPOLOGY_H_
#define TASKS_TASK_2_ALEXSEEV_D_RING_TOPOLOGY_RING_TOPOLOGY_H_

#include <mpi.h>
#include <iostream>
#include <ctime>

int getRandomInt(int min, int max);
bool isRceiverLarger(int sourceRank, int receiverRank);
bool isClockwise(int sourceRank, int receiverRank);
int nextRank(int rank);
int prevRank(int rank);
void sendMessage(int* message, int sourceRank, int receiverRank);

#endif  // TASKS_TASK_2_ALEXSEEV_D_RING_TOPOLOGY_RING_TOPOLOGY_H_
