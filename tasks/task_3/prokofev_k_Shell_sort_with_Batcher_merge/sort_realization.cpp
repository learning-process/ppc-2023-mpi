// Copyright 2023 Prokofev Kirill
#include "task_3/prokofev_k_Shell_sort_with_Batcher_merge/sort_realization.h"
#include <algorithm>
#include <limits>

const int intMin = INT_MIN;

void ShellSortSeq(std::vector<int>* vec) {
  int vecSize = vec->size();
  int gap = 1;
  while (gap < vecSize / 3) {
    gap = 3 * gap + 1;
  }
  while (gap > 0) {
    for (int i = gap; i < vecSize; ++i) {
      int temp = (*vec)[i];
      int j = i;
      while (j >= gap && (*vec)[j - gap] > temp) {
        (*vec)[j] = (*vec)[j - gap];
        j -= gap;
      }
      (*vec)[j] = temp;
    }
    gap /= 3;
  }
}

void ShellSortParallel(std::vector<int>* vec) {
  int numProc;
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  int rankProc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  int remainder = vec->size() % numProc;
  std::vector<int> preResultVector(vec->size() - remainder);
  int numElemInLocalVector = vec->size() / numProc;
  std::vector<int>localVector(numElemInLocalVector);
  MPI_Scatter(vec->data(), numElemInLocalVector, MPI_INT, localVector.data(),
    numElemInLocalVector, MPI_INT, 0, MPI_COMM_WORLD);
  ShellSortSeq(&localVector);
  MPI_Gather(localVector.data(), numElemInLocalVector, MPI_INT,
    preResultVector.data(), numElemInLocalVector, MPI_INT, 0, MPI_COMM_WORLD);
  if (rankProc == 0) {
    if (remainder > 0) {
      std::vector<int> remainderVect;
      int k = remainder;
      int remIndex = vec->size() - 1;
      while (k != 0) {
        remainderVect.push_back((*vec)[remIndex]);
        remIndex--;
        k--;
      }
      ShellSortSeq(&remainderVect);
      for (int i = 0; i < remainderVect.size(); i++) {
        preResultVector.push_back(remainderVect[i]);
      }
    }
    BatcherSort(&preResultVector);
    *vec = preResultVector;
  }
}

void BatcherMerge(std::vector<int>* vec, int l, int m, int r) {
  int n1 = m - l + 1;
  int n2 = r - m;
  std::vector<int> L(n1);
  std::vector<int> R(n2);
  for (int i = 0; i < n1; ++i)
    L[i] = (*vec)[l + i];
  for (int j = 0; j < n2; ++j)
    R[j] = (*vec)[m + 1 + j];
  int i = 0, j = 0, k = l;
  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      (*vec)[k] = L[i];
      ++i;
    } else {
      (*vec)[k] = R[j];
      ++j;
    }
    k++;
  }
  while (i < n1) {
    (*vec)[k] = L[i];
    ++i;
    ++k;
  }
}

void BatcherOddEvenMerge(std::vector<int>* vec, int start, int end, int dist) {
  if (dist > 1) {
    int mid = dist / 2;
    BatcherOddEvenMerge(vec, start, start + mid, mid);
    BatcherOddEvenMerge(vec, start + mid, end, mid);
    for (int i = start; i + mid < end; ++i) {
      if (i % dist < mid) {
        int l = i;
        int r = i + mid;
        int mergeEnd = std::min(r + mid, end);
        BatcherMerge(vec, l, r - 1, mergeEnd - 1);
      }
    }
  }
}

void BatcherSort(std::vector<int>* vec) {
  int n = vec->size();
  int powerOfTwo = 1;
  while (powerOfTwo < n) {
    powerOfTwo <<= 1;
  }
  if (powerOfTwo != n) {
    for (int i = 0; i < powerOfTwo - n; i++)
      vec->push_back(intMin);
    BatcherOddEvenMerge(vec, 0, powerOfTwo, powerOfTwo);
    vec->erase(std::remove(vec->begin(), vec->end(), intMin), vec->end());
    return;
  }
  BatcherOddEvenMerge(vec, 0, powerOfTwo, powerOfTwo);
}

std::vector<int> GenerateRandomVector(int n) {
  if (n <= 0)
    throw "Error";
  std::random_device dev;
  std::mt19937 generate(dev());
  std::vector<int> newVector(n);
  for (int i = 0; i < n; i++) {
    newVector[i] = generate() % n;
  }
  return newVector;
}
