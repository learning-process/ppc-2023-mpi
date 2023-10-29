#include "most_different.hpp"

std::vector<int> createRandomArray(size_t size, int low, int up) {
  std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> dist(low, up);

  std::vector<int> arr(size);
  for (auto& elem : arr) elem = dist(gen);

  return arr;
}

int sequentialFindMostDifferent(const std::vector<int>& arr) {
  size_t size = arr.size();
  int maxDiff = -1;

  if (size >= 2) {
    int absDiff = 0;
    for (size_t i = 0; i < size - 1; ++i) {
      absDiff = abs(arr[i] - arr[i + 1]);
      if (absDiff > maxDiff) maxDiff = absDiff;
    }
  }
  return maxDiff;
}

int parallelFindMostDifferent(const std::vector<int>& arr) {
  int sizeWorld = 0;
  int rank = 0;
  size_t sizeArr = arr.size();

  if (sizeArr < 2) return -1;

  MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int chunk = static_cast<int>(sizeArr / sizeWorld);
  const int tail = static_cast<int>(sizeArr % sizeWorld);
  
  std::vector<int> sendCount(sizeWorld);
  std::vector<int> displs(sizeWorld);

  for (int i = 0; i < sizeWorld - 1; ++i) {
    sendCount[i] = chunk + 1;
    displs[i] = i * chunk;
  }

  sendCount[sizeWorld - 1] =
      static_cast<int>((tail == 0) ? chunk : sizeArr - (sizeWorld - 1) * chunk);
  displs[sizeWorld - 1] = (sizeWorld - 1) * chunk;

  int globMaxDiff = 0;
  std::vector<int> localArr(sendCount[rank]);

  MPI_Scatterv(arr.data(), sendCount.data(), displs.data(), MPI_INT,
               localArr.data(), sendCount[rank], MPI_INT, 0, MPI_COMM_WORLD);

  int localMaxDiff = sequentialFindMostDifferent(localArr);

  MPI_Reduce(&localMaxDiff, &globMaxDiff, 1, MPI_INT, MPI_MAX, 0,
             MPI_COMM_WORLD);

  return globMaxDiff;
}
