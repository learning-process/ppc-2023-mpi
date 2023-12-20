// Copyright 2023 Durandin Vladimir
#ifndef TASKS_TASK_2_DURANDIN_V_MY_SCATTER_SCATTER_H_
#define TASKS_TASK_2_DURANDIN_V_MY_SCATTER_SCATTER_H_

#include <mpi.h>

#include <cstring>
#include <iostream>
#include <random>
#include <vector>

namespace my_scatter {
int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
                MPI_Comm comm);
int Binary_MPI_Scatter(const void *sendbuf, int sendcount,
                       MPI_Datatype sendtype, void *recvbuf, int recvcount,
                       MPI_Datatype recvtype, int root, MPI_Comm comm);
int Binary_Search(int left, int right, const int *rank, int *_parent,
                  int *_left_son, int *_right_son, int *_position,
                  const int *_type_size, const int *_recvcount);
}  // namespace my_scatter
std::vector<int64_t> create_random_vector_int64(const size_t size,
                                                const int64_t min,
                                                const int64_t max);
std::vector<int> create_random_vector_int(const size_t size, const int64_t min,
                                          const int64_t max);
std::vector<float> create_random_vector_float(const size_t size,
                                              const int64_t min,
                                              const int64_t max);
std::vector<double> create_random_vector_double(const size_t size,
                                                const int64_t min,
                                                const int64_t max);

#endif  // TASKS_TASK_2_DURANDIN_V_MY_SCATTER_SCATTER_H_
