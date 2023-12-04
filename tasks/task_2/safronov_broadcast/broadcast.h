

#include <iostream>
#include <mpi.h>

void bcast(void* data, int count, MPI_Datatype datatype,
    int root, MPI_Comm communicator);
