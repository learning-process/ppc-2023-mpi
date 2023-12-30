// Copyright 2023 Platonova Maria
#ifndef TASKS_TASK_2_PLATONOVA_M_SCATTER_METHOD_SCATTER_H_
#define TASKS_TASK_2_PLATONOVA_M_SCATTER_METHOD_SCATTER_H_

#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <utility>
#include <algorithm>

void MyScatter(const void* sendbuf, int sendcount,
                MPI_Datatype sendtype, void* recvbuf,
                int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);

std::vector<int> randomVector(int sizeVec, int minValue, int maxValue);
#endif  // TASKS_TASK_2_PLATONOVA_M_SCATTER_METHOD_SCATTER_H_
