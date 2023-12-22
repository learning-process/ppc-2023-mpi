// Copyright 2023 Sokolova Daria
#include "task_2/sokolova_d_scatter/scatter.h"
#include <mpi.h>
#include <algorithm>
void scatter(void* send_data, int send_count,
    MPI_Datatype send_datatype, void* recv_data,
    int recv_count, MPI_Datatype recv_datatype,
    int root, MPI_Comm communicator) {

    if (send_count <= 0 || recv_count <= 0 || root < 0)
        throw - 1;

    int size, rank;
    MPI_Comm_size(communicator, &size);
    MPI_Comm_rank(communicator, &rank);

    int send_elem_size, recv_elem_size;
    MPI_Type_size(send_datatype, &send_elem_size);
    MPI_Type_size(recv_datatype, &recv_elem_size);

    if (rank == root) {
        for (int i = 0; i < size; ++i) {
            if (i != root) {
                MPI_Send(static_cast<char*>(send_data)
                    + i * send_count * send_elem_size,
                    send_count, send_datatype, i, 0, communicator);
            } else {
                memcpy(recv_data, static_cast<char*>(send_data)
                    + i * send_count * send_elem_size,
                    send_count * send_elem_size);
            }
        }
    } else {
        MPI_Status status;
        MPI_Recv(recv_data, recv_count, recv_datatype,
            root, 0, communicator, &status);
    }
}
