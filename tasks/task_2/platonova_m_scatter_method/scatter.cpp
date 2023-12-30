// Copyright 2023 Platonova Maria
#include "task_2/platonova_m_scatter_method/scatter.h"

void MyScatter(const void* sendbuf, int sendcount, MPI_Datatype sendtype,
               void* recvbuf, int recvcount, MPI_Datatype recvtype,
               int root, MPI_Comm comm) {
    int rank;
    int size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int typeSize;
    MPI_Type_size(sendtype, &typeSize);

    if (rank == root) {
        for (int i = 0; i < size; ++i) {
            if (i != root) {
                MPI_Send(static_cast<const char*>(sendbuf) +
                         i * sendcount * typeSize, sendcount, sendtype,
                         i, 0, comm);
            } else {
                std::copy(static_cast<const char*>(sendbuf) + i *
                            sendcount * typeSize,
                          static_cast<const char*>(sendbuf) + (i + 1) *
                            sendcount * typeSize,
                          static_cast<char*>(recvbuf));
            }
        }
    } else {
        MPI_Status status;
        MPI_Recv(recvbuf, recvcount, recvtype, root, 0, comm, &status);
    }
}

std::vector<int> randomVector(int sizeVec, int minValue, int maxValue) {
    std::random_device rd;
    std::default_random_engine gen{rd()};
    std::uniform_int_distribution<> random(minValue, maxValue);

    std::vector<int> vec(sizeVec);

    for (int &value : vec)
        value = random(gen);

    return vec;
}
