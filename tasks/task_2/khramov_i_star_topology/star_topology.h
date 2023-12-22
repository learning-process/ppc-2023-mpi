// Copyright 2023 Khramov Ivan
#ifndef TASKS_TASK_2_KHRAMOV_I_STAR_TOPOLOGY_STAR_TOPOLOGY_H_
#define TASKS_TASK_2_KHRAMOV_I_STAR_TOPOLOGY_STAR_TOPOLOGY_H_

#include <mpi.h>

void sendDataStar(
    MPI_Comm world,
    int master,
    int source,
    int receiver,
    int tag,
    void* data,
    MPI_Datatype datatype);

#endif  // TASKS_TASK_2_KHRAMOV_I_STAR_TOPOLOGY_STAR_TOPOLOGY_H_
