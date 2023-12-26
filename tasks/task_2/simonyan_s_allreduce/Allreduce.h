// Copyright 2023 Simonyan Suren
#ifndef TASKS_TASK_2_SIMONYAN_S_ALLREDUCE_ALLREDUCE_H_
#define TASKS_TASK_2_SIMONYAN_S_ALLREDUCE_ALLREDUCE_H_

#include <mpi.h>
#include <iostream>

int My_AllReduce(const void* sendbuf, void* recvbuf, int count,
    MPI_Datatype datatype, MPI_Op  op, int root, MPI_Comm comm);

template <typename T>
void operation(void* buf, void* recvbuf, MPI_Op op) {
    T& buf_val = *reinterpret_cast<T*>(buf);
    T& recvbuf_val = *reinterpret_cast<T*>(recvbuf);
    if (op == MPI_MAX) {
        if (buf_val > recvbuf_val) recvbuf_val = buf_val;
    } else if (op == MPI_MIN) {
        if (buf_val < recvbuf_val) recvbuf_val = buf_val;
    } else if (op == MPI_SUM) {
        recvbuf_val += buf_val;
    } else if (op == MPI_PROD) {
        recvbuf_val *= buf_val;
    }
}

#endif  // TASKS_TASK_2_SIMONYAN_S_ALLREDUCE_ALLREDUCE_H_
