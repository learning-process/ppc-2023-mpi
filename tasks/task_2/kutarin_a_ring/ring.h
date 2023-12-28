// Copyright 2023 Kutarin Aleksandr
#ifndef TASKS_TASK_2_KUTARIN_A_RING_RING_H_
#define TASKS_TASK_2_KUTARIN_A_RING_RING_H_

#include <mpi.h>
#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <cstdlib>
#include <functional>
#include <algorithm>

int prevRank(int rank);
int nextRank(int rank);
bool isDownTime(int rank, int source, int receiver);
void sendMessage(int* message, int source, int receiver);


#endif  // TASKS_TASK_2_KUTARIN_A_RING_RING_H_
