#include "most_different.hpp"

int findMostDifferent(std::vector<int>& arr, int argc, char** argv) {
  int rank = 0;
  int sizeWorld = 0;
  int localMaxDiff = -1;
  size_t sizeArray = arr.size();

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);

  size_t step = sizeArray / sizeWorld;
  size_t start = rank * step;
  size_t stop = start + step;

  if (rank == sizeWorld - 1 && sizeArray % sizeWorld != 0) {
    stop += sizeArray % sizeWorld - 1;
  }

  for (size_t i = start; i < stop; ++i) {
    int absDiff = std::abs(arr[i] - arr[i + 1]);
    if (absDiff > localMaxDiff) localMaxDiff = absDiff;
  }

  if (rank == 0) {
    int globalMaxDiff = localMaxDiff;

    for (int i = 1; i < sizeWorld; ++i) {
      int recvMaxDiff = 0;
      MPI_Recv(&recvMaxDiff, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
               MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

      if (recvMaxDiff > globalMaxDiff) globalMaxDiff = recvMaxDiff;
    }
  } else {
    MPI_Send(&localMaxDiff, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

  return MPI_Finalize();
}