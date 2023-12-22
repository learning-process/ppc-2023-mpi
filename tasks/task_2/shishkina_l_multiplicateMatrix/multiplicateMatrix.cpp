// Copyright 2023 Shishkina Valeria
#include "task_2/shishkina_l_multiplicateMatrix/multiplicateMatrix.h"

std::vector<int> matrixMultiplicate(std::vector<int>* A, std::vector<int>* B,
                                    std::vector<int>* C, int maxSize,
                                    int count) {
  for (int i = 0; i < count; i++) {
    for (int j = 0; j < maxSize; j++) {
      (*C)[i * maxSize + j] = 0;
      for (int k = 0; k < maxSize; k++) {
        (*C)[i * maxSize + j] += (*A)[i * maxSize + k] * (*B)[k * maxSize + j];
      }
    }
  }
  return (*C);
}

std::vector<int> matrixMultiplicationParallel(std::vector<int>* A,
                                              std::vector<int>* B,
                                              std::vector<int>* C,
                                              int maxSize) {
  int rank = 0;
  int size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int portion = maxSize / size;
  int remainder = maxSize % size;
  std::vector<int> count, sendcount, displ;
  count.resize(size);
  sendcount.resize(size);
  displ.resize(size);
  for (int i = 0; i < size; i++) {
    count[i] = portion;
    if (i < remainder) {
      count[i]++;
    }
  }
  for (int i = 0; i < size; i++) {
    int tmp = count[i] * maxSize;
    sendcount[i] = tmp;
  }

  int _displ = 0;
  for (int i = 0; i < size; i++) {
    displ[i] = _displ;
    _displ += sendcount[i];
  }

  std::vector<int> localA(sendcount[rank]);

  MPI_Scatterv((*A).data(), sendcount.data(), displ.data(), MPI_INT,
               localA.data(), sendcount[rank], MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Bcast((*B).data(), maxSize * maxSize, MPI_INT, 0, MPI_COMM_WORLD);

  std::vector<int> localC(sendcount[rank]);
  localC = matrixMultiplicate(&localA, &(*B), &localC, maxSize, count[rank]);

  MPI_Gatherv(localC.data(), sendcount[rank], MPI_INT, (*C).data(),
              sendcount.data(), displ.data(), MPI_INT, 0, MPI_COMM_WORLD);
  return (*C);
}
