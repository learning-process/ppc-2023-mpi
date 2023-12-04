// Copyright 2023 Safronov Mikhail
#ifndef TASKS_TASK_2_SAFRONOV_BROADCAST_BROADCAST_H_
#define TASKS_TASK_2_SAFRONOV_BROADCAST_BROADCAST_H_

#include <mpi.h>
#include <iostream>

void bcast(void* data, int count, MPI_Datatype datatype,
    int root, MPI_Comm communicator);

#endif  // TASKS_TASK_2_SAFRONOV_BROADCAST_BROADCAST_H_
