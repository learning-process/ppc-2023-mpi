// Copyright 2023 Shemyakina Alesya
#ifndef TASKS_TASK_2_SHEMYAKINA_A_STAR_TOPOLOGY_STAR_TOPOLOGY_H_
#define TASKS_TASK_2_SHEMYAKINA_A_STAR_TOPOLOGY_STAR_TOPOLOGY_H_

#include <mpi.h>

void StarData(MPI_Comm world, int master, int source, int receiver, int tag, void* data, MPI_Datatype datatype);

#endif  // TASKS_TASK_2_SHEMYAKINA_A_STAR_TOPOLOGY_STAR_TOPOLOGY_H_
