// Copyright 2023 Khavronina Alexandra

#ifndef TASKS_TASK_2_KHAVRONINA_A_SCATTER_SCATTER_H_
#define TASKS_TASK_2_KHAVRONINA_A_SCATTER_SCATTER_H_

#include <mpi.h>

void scatter(void* send_buf, int send_count, MPI_Datatype send_type, void* recv_buf,
     int recv_count, MPI_Datatype recv_type, int root, MPI_Comm comm);

#endif  // TASKS_TASK_2_KHAVRONINA_A_SCATTER_SCATTER_H_
