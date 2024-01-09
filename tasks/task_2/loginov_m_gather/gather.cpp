// Copyright 2023 Loginov Maxim
#include "task_2/loginov_m_gather/gather.h"

int Gather(const void* sbuf, int scount, MPI_Datatype stype, void* rbuf,
  int rcount, MPI_Datatype rtype, int root, MPI_Comm comm) {
    int numProc;
    MPI_Comm_size(comm, &numProc);
    int rankProc;
    MPI_Comm_rank(comm, &rankProc);
    if (!(root >= 0 && root < numProc)) return MPI_ERR_ROOT;
    if ((scount <= 0) || (rcount <= 0) || (rcount != scount))
        return MPI_ERR_COUNT;
    if (!(stype == MPI_INT || stype == MPI_FLOAT || stype == MPI_DOUBLE))
      return MPI_ERR_TYPE;
    int sizeofType = 0;
    int father = rankProc - 1;
    int son = rankProc + 1;
    MPI_Type_size(stype, &sizeofType);
    size_t bytes = static_cast<size_t>(scount * sizeofType);
    void* tmpBuf = std::malloc(bytes * (numProc - rankProc));
    std::memcpy(tmpBuf, sbuf, bytes);
    if (numProc == 1) {
      std::memcpy(rbuf, tmpBuf, bytes);
    } else {
      if (rankProc != 0) {
        if (son < numProc) {
          MPI_Recv(reinterpret_cast<char*>(tmpBuf) + 1 * rcount * sizeofType,
            rcount * (numProc - rankProc - 1),
            stype, son, 0, comm, MPI_STATUS_IGNORE);
          MPI_Send(tmpBuf, scount * (numProc - rankProc),
            stype, father, 0, comm);
        } else {
          MPI_Send(tmpBuf, scount, stype, father, 0, comm);
        }
      } else {
        MPI_Recv(reinterpret_cast<char*>(tmpBuf) + 1 * rcount * sizeofType,
          rcount * (numProc - rankProc - 1),
          stype, son, 0, comm, MPI_STATUS_IGNORE);
        if (rankProc == root) {
          std::memcpy(rbuf, tmpBuf, bytes * (numProc - rankProc));
        } else {
          MPI_Send(tmpBuf, scount * numProc, stype, root, 0, comm);
        }
      }
      if (rankProc == root && rankProc != 0) {
        MPI_Recv(rbuf, scount * numProc, stype, 0, 0, comm, MPI_STATUS_IGNORE);
      }
    }
    free(tmpBuf);
    return MPI_SUCCESS;
}
