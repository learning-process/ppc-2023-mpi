// Copyright 2023 Moiseev Nikita
#ifndef TASKS_TASK_2_MOISEEV_N_LINEAR_TOPOLOGY_LINEAR_TOPOLOGY_H_
#define TASKS_TASK_2_MOISEEV_N_LINEAR_TOPOLOGY_LINEAR_TOPOLOGY_H_

#include <mpi.h>

int getForwardRank(int currentRank, bool isForward);
int getBackwardRank(int currentRank, bool isForward);
bool isInRoute(int currentRank, int sourceRank, int destRank, bool isForward);
void sendDataLinear(void* data, int count, MPI_Datatype datatype,
                int sourceRank, int destRank, int tag, MPI_Comm comm);

#endif  // TASKS_TASK_2_MOISEEV_N_LINEAR_TOPOLOGY_LINEAR_TOPOLOGY_H_
