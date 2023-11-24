// Copyright 2023 Kostin Artem
#ifndef TASKS_TASK_2_KOSTIN_A_CIRCLE_TOPOLOGY_CIRCLE_TOPOLOGY_H_
#define TASKS_TASK_2_KOSTIN_A_CIRCLE_TOPOLOGY_CIRCLE_TOPOLOGY_H_

#include <mpi.h>
#include <vector>
#include <string>

int next_rank(int r);
int prev_rank(int r);
void send_data(int* data, int fromProc, int toProc);

#endif  // TASKS_TASK_2_KOSTIN_A_CIRCLE_TOPOLOGY_CIRCLE_TOPOLOGY_H_
