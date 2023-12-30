// Copyright 2023 Balyasov Ivan
#include "task_2/balyasov_reduce/my_reduce.h"

int reduce(const void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
           MPI_Op op, int root, MPI_Comm comm) {
  int rank = 0;
  int world_size = 0;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &world_size);

  if (!(root >= 0 && root < world_size)) return MPI_ERR_ROOT;

  if (count == 0) return MPI_ERR_COUNT;

  if (!(datatype == MPI_INT || datatype == MPI_FLOAT || datatype == MPI_DOUBLE))
    return MPI_ERR_TYPE;

  if (!(op == MPI_MAX || op == MPI_MIN || op == MPI_SUM || op == MPI_PROD ||
        op == MPI_LAND || op == MPI_LOR))
    return MPI_ERR_OP;

  if (rank == root) {
    int sizeof_type = 0;
    MPI_Type_size(datatype, &sizeof_type);

    size_t bytes = static_cast<size_t>(sizeof_type * count);
    void* buf = std::malloc(bytes);

    std::memcpy(buf, sendbuf, bytes);
    std::memcpy(recvbuf, sendbuf, bytes);

    for (int i = 0; i < world_size - 1; ++i) {
      MPI_Recv(buf, count, datatype, MPI_ANY_SOURCE, 0, comm,
               MPI_STATUS_IGNORE);

      if (datatype == MPI_INT) {
        operation<int>(buf, recvbuf, op);
      } else if (datatype == MPI_FLOAT) {
        operation<float>(buf, recvbuf, op);
      } else if (datatype == MPI_DOUBLE) {
        operation<double>(buf, recvbuf, op);
      }
    }
  } else {
    MPI_Send(sendbuf, count, datatype, root, 0, comm);
  }

  MPI_Barrier(comm);

  return MPI_SUCCESS;
}
