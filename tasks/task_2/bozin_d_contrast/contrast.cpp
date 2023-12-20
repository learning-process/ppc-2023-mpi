// Copyright 2023 Bozin Dmitry

#include "task_2/bozin_d_contrast/contrast.h"

std::vector<int> getRandomVector(int  size) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<int> vector(size);
  for (int i = 0; i < size; i++) { vector[i] = gen() % 100; }
  return vector;
}

std::pair<int, int> getSequentialMaxMin(std::vector<int> vector) {
  const int size = vector.size();
  if (size > 0) {
    int max = vector[0], min = vector[0];
    for (int i = 1; i < size; i++) {
      if (vector[i] > max) max = vector[i];
      if (vector[i] < min) min = vector[i];
    }

    return std::make_pair(min, max);
  }
  else {
    return std::make_pair(2147483647, 0);
  }
}

int Clamp(int a, int l, int r) {
  if (a > r) return r;
  if (a < l) return l;
  return a;
}

std::vector<int> getSequentialContrast(std::vector<int> vector, const int max, const int min, const int B) { // NOLINT
  for (int i = 0; i < vector.size(); i++) {
    vector[i] = Clamp((vector[i] - min) * 255 / (max - min) + B, 0, 255);
  }
  return vector;
}

std::vector<std::vector<int>> SequentialContrast(std::vector<std::vector<int>> vector, int width, int height) {
  const int B = 50;
  int globalMax = 0;
  int globalMin = 0;
  std::vector<int> result;
  std::vector<std::vector<int>> realResult;
  std::vector<int> globalVec;

  globalVec = std::vector<int>(width * height);
  result = std::vector<int>(width * height);
  realResult = std::vector<std::vector<int>>(width);

  for (int i = 0; i < width; i++) {
    realResult[i] = std::vector<int>(height);
  }

  for (int i = 0; i < width * height; i++) {
    globalVec[i] = vector[static_cast<int>(i / height)][i % height];
  }

  std::pair<int, int> minMax = (getSequentialMaxMin(globalVec));

  globalMax = minMax.second;
  globalMin = minMax.first;

  result = getSequentialContrast(globalVec, globalMax, globalMin, B);

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      realResult[i][j] = result[i * height + j];
    }
  }

  return realResult;
}


std::vector<std::vector<int>> ParallelContrast(std::vector<std::vector<int>> vector, int width, int height) {
  int size, rank;
  const int B = 50;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int delta = static_cast<int>((width * height) / size);
  const int remainder = (width * height) % size;
  std::vector<int> result;
  std::vector<std::vector<int>> realResult;
  int* sendCount = new int[size];
  int* displs = new int[size];

  std::vector<int> globalVec = std::vector<int>(width * height);

  sendCount[0] = delta + static_cast<int>(remainder > 0);
  displs[0] = 0;

  if (remainder > 0) {
    for (int proc = 1; proc < remainder; proc++) {
      sendCount[proc] = delta + 1;
      displs[proc] = proc * delta + proc;
    }
    for (int proc = remainder; proc < size; proc++) {
      if (proc > 0) {
        sendCount[proc] = delta;
        displs[proc] = proc * delta + remainder;
      }
    }
  }
  else {
    for (int proc = 1; proc < size; proc++) {
      sendCount[proc] = delta;
      displs[proc] = proc * delta;
    }
  }


  result = std::vector<int>(width * height);
  realResult = std::vector<std::vector<int>>(width);

  for (int i = 0; i < width; i++) {
    realResult[i] = std::vector<int>(height);
  }

  if (rank == 0) {
    for (int i = 0; i < width * height; i++) {
      globalVec[i] = vector[static_cast<int>(i / height)][i % height];
    }
  }

  int globalMax = 0;
  int globalMin = 0;

  std::vector<int> localVec(sendCount[rank]);

  MPI_Scatterv(globalVec.data(), sendCount, displs, MPI_INT,
    localVec.data(), sendCount[rank], MPI_INT, 0, MPI_COMM_WORLD);

  std::pair<int, int> minMax = (getSequentialMaxMin(localVec));

  MPI_Reduce(&(minMax.first), &globalMin, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
  MPI_Reduce(&(minMax.second), &globalMax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

  MPI_Bcast(&globalMax, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&globalMin, 1, MPI_INT, 0, MPI_COMM_WORLD);

  localVec = getSequentialContrast(localVec, globalMax, globalMin, B);

  MPI_Gatherv(localVec.data(), sendCount[rank], MPI_INT, result.data(),
    sendCount, displs, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
        realResult[i][j] = result[i * height + j];
      }
    }
  }

  return realResult;
}
