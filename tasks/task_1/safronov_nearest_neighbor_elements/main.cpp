#include <gtest/gtest.h>
#include <vector>
#include "nearest_neighbor_elements.h"
#include "mpi.h"

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  VectorProcessor processor;
  
  std::vector<int> inputVector = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  processor.findClosestNeighbors(inputVector);
  
  MPI_Finalize();
  return 0;
}