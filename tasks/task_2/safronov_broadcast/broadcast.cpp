#include "broadcast.h"

void bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator) {
    int rank, size;
    MPI_Comm_rank(communicator, &rank);
    MPI_Comm_size(communicator, &size);
    
    if (rank == root) {
        for (int i = 0; i < size; i++) {
            if (i != root) {  // Exclude the root process
                MPI_Send(data, count, datatype, i, 0, communicator);
            }
        }
    } else {
        MPI_Recv(data, count, datatype, root, 0, communicator, MPI_STATUS_IGNORE);
    }
}



