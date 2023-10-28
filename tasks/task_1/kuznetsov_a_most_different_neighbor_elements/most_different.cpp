#include "most_different.hpp"

int findMostDifferent(std::vector<int>& arr) {
  int rank = 0;
  int sizeWorld = 0;
  int localMaxDiff = -1;
  int globalMaxDiff = -1;
  size_t sizeArray = arr.size();

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);

  if (rank == 0) {
    std::cout << "PROC: " << sizeWorld << " SIZE_ARR: " << sizeArray << '\n';
  }

  size_t step = sizeArray / sizeWorld;
  size_t start = rank * step;
  //size_t stop = start + step;  
  size_t stop = (rank == sizeWorld - 1) ? sizeArray - 1 : start + step;


  std::cout << "Rank: " << rank << " Start: " << start << " Stop: " << stop
            << '\n'; 

  for (size_t i = start; i < stop; ++i) {
    int absDiff = std::abs(arr[i] - arr[i + 1]);
    if (absDiff > localMaxDiff) localMaxDiff = absDiff;
    std::cout << "CMP " << i << ' ' << i + 1 << '\n';
  }

  if (rank == 0) {
    globalMaxDiff = localMaxDiff;

    for (int i = 1; i < sizeWorld; ++i) {
      int recvMaxDiff = 0;
      MPI_Recv(&recvMaxDiff, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
               MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

      if (recvMaxDiff > globalMaxDiff) globalMaxDiff = recvMaxDiff;
      
    }
    std::cout << "GLOBAL_DIFF: " << globalMaxDiff << '\n';
  } else {
    std::cout << "Rank " << rank << " LocalDiff " << localMaxDiff << '\n'; 
    MPI_Send(&localMaxDiff, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

  std::cout << "RETURNED: " << globalMaxDiff << '\n';
  return globalMaxDiff;
}