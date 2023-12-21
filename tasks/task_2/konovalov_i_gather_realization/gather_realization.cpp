// Copyright 2023 Konovalov Igor
#include "task_2/konovalov_i_gather_realization/gather_realization.h"

int custom_gather(const void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf,
  int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm) {
  int numProc;
  int rankProc;
  
  MPI_Comm_size(comm, &numProc);
  MPI_Comm_rank(comm, &rankProc);

  if ((root < 0) || (root >= numProc)) {
    return MPI_ERR_ROOT;
  }

  if ((sendcount <= 0) || (recvcount <= 0) || (recvcount != sendcount)) {
    return MPI_ERR_COUNT;
  }

  if ((sendtype != MPI_INT) && (sendtype != MPI_FLOAT) && (sendtype != MPI_DOUBLE)) {
    return MPI_ERR_TYPE;
  }

  int size;
  int prevProc = rankProc - 1;
  int nextProc = rankProc + 1;
  MPI_Type_size(sendtype, &sizeofType);
  size_t bytes = static_cast<size_t>(sendcount * sizeofType);
  void* buf = std::malloc(bytes * (numProc - rankProc));
  std::memcpy(buf, sendbuf, bytes);

  if (numProc == 1) {
    std::memcpy(recvbuf, buf, bytes);
  } else {
    if (rankProc != 0) {
      if (nextProc < numProc) {
        MPI_Recv(static_cast<char*>(buf) + 1 * recvcount * size, recvcount * (numProc - rankProc - 1),
          sendtype, nextProc, 0, comm, MPI_STATUS_IGNORE);
        MPI_Send(buf, sendcount * (numProc - rankProc), sendtype, prevProc, 0, comm);
      } else {
        MPI_Send(buf, sendcount, sendtype, prevProc, 0, comm);
      }
    } else {
      MPI_Recv(static_cast<char*>(buf) + 1 * recvcount * size, recvcount * (numProc - rankProc - 1), sendtype,
        nextProc, 0, comm, MPI_STATUS_IGNORE);
      if (rankProc == root) {
        std::memcpy(recvbuf, buf, bytes * (numProc - rankProc));
      } else {
        MPI_Send(buf, sendcount * numProc, sendtype, root, 0, comm);
      }
    }
    if (rankProc == root && rankProc != 0) {
      MPI_Recv(recvbuf, sendcount * numProc, sendtype, 0, 0, comm, MPI_STATUS_IGNORE);
    }
  }
  free(buf);
  return MPI_SUCCESS;
}
