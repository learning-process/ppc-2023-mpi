// Copyright 2023 Shubin Mikhail
#ifndef TASKS_TASK_2_SHUBIN_M_SCATTER_CUSTOM_SCATTER_H_
#define TASKS_TASK_2_SHUBIN_M_SCATTER_CUSTOM_SCATTER_H_

#include <mpi.h>
#include <cmath>
#include <vector>
#include <iostream>

int custom_scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                   void *recvbuf, int recvcount,
                   MPI_Datatype recvtype, int root,
                   MPI_Comm comm);
int test_func(int val, bool custom_sc);
float test_func(float val, bool custom_sc);
double test_func(double val, bool custom_sc);
int test_func(const std::vector<int>& val_vec,
              int batch_size, bool custom_sc);
double test_func(const std::vector<double>& val_vec,
                 int batch_size, bool custom_sc);

#endif  // TASKS_TASK_2_SHUBIN_M_SCATTER_CUSTOM_SCATTER_H_
