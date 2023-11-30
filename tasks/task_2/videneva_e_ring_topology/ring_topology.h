// Copyright 2023 Videneva Ekaterina
#ifndef TASKS_TASK_2_VIDENEVA_E_RING_TOPOLOGY_RING_TOPOLOGY_H_
#define TASKS_TASK_2_VIDENEVA_E_RING_TOPOLOGY_RING_TOPOLOGY_H_

#include <mpi.h>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <functional>
#include <string>
#include <random>

int nextRank(int rank);
int prevRank(int rank);
bool isDownTime(int rank, int source, int receiver);
void sendMessage(int* message, int source, int receiver);


#endif  // TASKS_TASK_2_VIDENEVA_E_RING_TOPOLOGY_RING_TOPOLOGY_H_