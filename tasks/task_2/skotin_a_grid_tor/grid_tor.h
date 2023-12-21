// Copyright 2023 Skotin Alexander
#ifndef TASKS_TASK_2_SKOTIN_A_GRID_TOR_GRID_TOR_H_
#define TASKS_TASK_2_SKOTIN_A_GRID_TOR_GRID_TOR_H_
#include <mpi.h>

const int GRID_SIZE = 5;

class TorusGrid {
 public:
    TorusGrid();
    void calculateNeighbors();
    void sendToNeighbor(void* data, int count, MPI_Datatype datatype,
                        int source, int destination, int tag);

 private:
    int rank_, size_;
    int row_, col_;
    MPI_Comm row_comm_, col_comm_;
    int neighbor_north_, neighbor_south_, neighbor_east_, neighbor_west_;

    int determineNextNode(int source, int destination) const;
};
#endif  // TASKS_TASK_2_SKOTIN_A_GRID_TOR_GRID_TOR_H_
