// Copyright 2023 Prokofev Kirill
#include "task_2/prokofev_k_gather_realization/gather_realization.h"

int Gather(const void* sbuf, int scount, MPI_Datatype stype, void* rbuf, int rcount,
  MPI_Datatype rtype, int root, MPI_Comm comm) {
  int numProc;
  MPI_Comm_size(comm, &numProc);
  int rankProc;
  MPI_Comm_rank(comm, &rankProc);
  if (!(root >= 0 && root < numProc)) return MPI_ERR_ROOT;
  if ((scount <= 0) || (rcount <= 0) || (rcount != scount)) return MPI_ERR_COUNT;
  if (!(stype == MPI_INT || stype == MPI_FLOAT || stype == MPI_DOUBLE))
    return MPI_ERR_TYPE;
  if (rankProc == root) {
    int sizeofType = 0;
    MPI_Type_size(stype, &sizeofType);
    size_t bytes = static_cast<size_t>(scount * sizeofType);
    std::memcpy(rbuf, sbuf, bytes);
    for (int i = 0; i < numProc; ++i) {
      if (i != rankProc)
        MPI_Recv(reinterpret_cast<char*>(rbuf) + i * rcount * sizeofType, rcount, rtype, i, 0, comm, MPI_STATUS_IGNORE);
    }
  }else
    MPI_Send(sbuf, scount, stype, root, 0, comm);
  return MPI_SUCCESS;
}


