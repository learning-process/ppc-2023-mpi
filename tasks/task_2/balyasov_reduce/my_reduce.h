// Copyright 2023 Balyasov Ivan
#ifndef TASKS_TASK_2_BALYASOV_REDUCE_MY_REDUCE_H_
#define TASKS_TASK_2_BALYASOV_REDUCE_MY_REDUCE_H_

#include <math.h>
#include <mpi.h>

#include <cstdlib>
#include <cstring>

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
  } else if (op == MPI_LAND) {
    recvbuf_val = static_cast<bool>(recvbuf_val) && static_cast<bool>(buf_val);
  } else if (op == MPI_LOR) {
    recvbuf_val = static_cast<bool>(recvbuf_val) || static_cast<bool>(buf_val);
  }
}

int reduce(const void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
           MPI_Op op, int root, MPI_Comm comm);

#endif  // TASKS_TASK_2_BALYASOV_REDUCE_MY_REDUCE_H_
