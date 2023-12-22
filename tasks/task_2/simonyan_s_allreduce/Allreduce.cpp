// Copyright 2023 Suren Simonyan


#include "Allreduce.h"
#include <cstring>

int My_AllReduce(const void* sendbuf, void* recvbuf, int count,
    MPI_Datatype datatype, MPI_Op  op, int root, MPI_Comm comm) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    if (root<0 || root>ProcNum) {
        return MPI_ERR_ROOT;
    }
    if (!(datatype == MPI_INT ||
        datatype == MPI_FLOAT || datatype == MPI_DOUBLE))
        return MPI_ERR_TYPE;
    if (!(op == MPI_MAX || op == MPI_MIN || op == MPI_SUM || op == MPI_PROD))
        return MPI_ERR_OP;
    if (ProcRank == root) {
        int sizeof_type = 0;
        MPI_Type_size(datatype, &sizeof_type);

        size_t bytes = static_cast<size_t>(sizeof_type * count);
        void* buf = std::malloc(bytes);

        memcpy(buf, sendbuf, bytes);
        memcpy(recvbuf, sendbuf, bytes);
        for (int i = 0; i < ProcNum - 1; ++i) {
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
        for (int i = 0; i <= ProcNum - 1; i++) {
            if (i != root) {
                MPI_Send(recvbuf, count, datatype, i, 0, comm);
            }
        }
    } else {
        MPI_Send(sendbuf, count, datatype, root, 0, comm);
    }

    if (ProcRank != root) {
        MPI_Recv(recvbuf, count, datatype, 0, 0, comm,
            MPI_STATUS_IGNORE);
    }
    MPI_Barrier(comm);
    return MPI_SUCCESS;
}
