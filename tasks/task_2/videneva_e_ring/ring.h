// Copyright 2023 Videneva Ekaterina
#ifndef TASKS_TASK_2_VIDENEVA_E_RING_RING_H_
#define TASKS_TASK_2_VIDENEVA_E_RING_RING_H_

#include <mpi.h>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <random>
#include <functional>
#include <string>


int generateRandomNumbers(int min, int max);
bool isRceiverLarger(int sourceRank, int receiverRank);
bool isClockwise(int sourceRank, int receiverRank);
void sendMessage(int* message, int sourceRank, int receiverRank);


#endif  // TASKS_TASK_2_VIDENEVA_E_RING_RING_H_
