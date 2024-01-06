// Copyright 2023 Kashirin Alexander
#include "task_2/kashirin_a_gather/gather.h"

int Gather(const void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf,
    int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm) {
    int numProc;
    int rankProc;
    MPI_Comm_size(comm, &numProc);
    MPI_Comm_rank(comm, &rankProc);
    int sizeofType = 0;
    MPI_Type_size(sendtype, &sizeofType);

    if (!(sendtype == MPI_INT || sendtype == MPI_FLOAT || sendtype == MPI_DOUBLE) ||
        (sendcount <= 0 || recvcount <= 0 || root < 0 || root >= numProc)) throw - 1;

    int previous = rankProc - 1;
    int next = rankProc + 1;
    int numbytes = sendcount * sizeofType;
    void* tmpBuf = std::malloc(numbytes * (numProc - rankProc));
    std::memcpy(tmpBuf, sendbuf, numbytes);

    if (numProc == 1) {
        std::memcpy(recvbuf, tmpBuf, numbytes);
    } else {
        if (rankProc != 0) {
            if (next < numProc) {
                MPI_Recv(reinterpret_cast<char*>(tmpBuf) + 1 * recvcount * sizeofType,
                    recvcount * (numProc - rankProc - 1),
                    sendtype, next, 0, comm, MPI_STATUS_IGNORE);
                MPI_Send(tmpBuf, sendcount * (numProc - rankProc),
                    sendtype, previous, 0, comm);
            } else {
                MPI_Send(tmpBuf, sendcount, sendtype, previous, 0, comm);
            }
        } else {
            MPI_Recv(reinterpret_cast<char*>(tmpBuf) + 1 * recvcount * sizeofType,
                recvcount * (numProc - rankProc - 1),
                sendtype, next, 0, comm, MPI_STATUS_IGNORE);
            if (rankProc == root) {
                std::memcpy(recvbuf, tmpBuf, numbytes * (numProc - rankProc));
            } else {
                MPI_Send(tmpBuf, sendcount * numProc, sendtype, root, 0, comm);
            }
        }
        if (rankProc == root && rankProc != 0) {
            MPI_Recv(recvbuf, sendcount * numProc, sendtype, 0, 0, comm, MPI_STATUS_IGNORE);
        }
    }
    free(tmpBuf);
    return MPI_SUCCESS;
}
