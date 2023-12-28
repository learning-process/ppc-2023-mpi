// Copyright 2023 Troitskiy Alexandr

#ifndef TASKS_TASK_2_TROITSKIY_A_LIN_TOPOLOGY_LIN_TOPOLOGY_H_
#define TASKS_TASK_2_TROITSKIY_A_LIN_TOPOLOGY_LIN_TOPOLOGY_H_

bool IsInRoute(int current, int source, int destination, bool forward_direction);

int GetNextNode(int current, bool forward_direction);

int GetPreviousNode(int current, bool forward_direction);

void SendDataLinearTopology(void *data, int count, MPI_Datatype datatype, int source,
                            int destination, int tag, MPI_Comm comm);

#endif  //  TASKS_TASK_2_TROITSKIY_A_LIN_TOPOLOGY_LIN_TOPOLOGY_H_
