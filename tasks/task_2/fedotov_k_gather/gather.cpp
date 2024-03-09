// Copyright 2024 Fedotov Kirill
#include <mpi.h>
#include <algorithm>
#include "task_2/fedotov_k_gather/gather.h"

using std::copy;

// MPI_Gather
int MPI_Gather_c(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                 void *recvbuf, int recvcount, MPI_Datatype recvtype,
                 int root, MPI_Comm comm) {
    int size, rank;
    int sendtype_size, recvtype_size;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    if (MPI_Type_size(sendtype, &sendtype_size) == MPI_ERR_TYPE) {
        return MPI_ERR_TYPE;
    }
    if (MPI_Type_size(recvtype, &recvtype_size) == MPI_ERR_TYPE) {
        return MPI_ERR_TYPE;
    }

    if (rank == root) {
        copy(reinterpret_cast<const char*>(sendbuf),
             reinterpret_cast<const char*>(sendbuf) + sendtype_size * sendcount,
             reinterpret_cast<char*>(recvbuf) + recvcount * recvtype_size * root);
        for (int i = 0; i < size; ++i) {
            if (i == rank)
                continue;
            MPI_Status status;
            MPI_Recv(reinterpret_cast<char*>(recvbuf) + recvcount * recvtype_size * i, recvcount,
                     recvtype, i, 0, comm, &status);
        }
    } else {
        MPI_Send(sendbuf, sendcount, sendtype, root, 0, comm);
    }

    return MPI_SUCCESS;
}
