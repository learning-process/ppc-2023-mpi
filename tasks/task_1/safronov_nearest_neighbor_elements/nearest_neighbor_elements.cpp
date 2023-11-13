#include "nearest_neighbor_elements.h"
#include <iostream>
#include <mpi.h>

VectorProcessor::VectorProcessor() {
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
}

void VectorProcessor::findClosestNeighbors(const std::vector<int>& inputVector) {
  int vectorSize = inputVector.size();
  int localSize = vectorSize / size;
  
  // Разделение вектора на части для каждого процесса
  std::vector<int> localVector(localSize);
  MPI_Scatter(inputVector.data(), localSize, MPI_INT,
              localVector.data(), localSize, MPI_INT, 0, MPI_COMM_WORLD);
  
  // Нахождение ближайших соседей
  int leftNeighbor, rightNeighbor;
  
  if (rank == 0) {
    leftNeighbor = MPI_PROC_NULL; // Нет левого соседа
    MPI_Send(&localVector[localSize - 1], 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    MPI_Recv(&rightNeighbor, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  } else if (rank == size - 1) {
    rightNeighbor = MPI_PROC_NULL; // Нет правого соседа
    MPI_Send(&localVector[0], 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
    MPI_Recv(&leftNeighbor, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  } else {
    MPI_Send(&localVector[0], 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
    MPI_Recv(&leftNeighbor, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(&localVector[localSize - 1], 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    MPI_Recv(&rightNeighbor, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  
  // Вывод результатов
  std::cout << "Process " << rank << " - Left neighbor: " << leftNeighbor
            << ", Right neighbor: " << rightNeighbor << std::endl;
}