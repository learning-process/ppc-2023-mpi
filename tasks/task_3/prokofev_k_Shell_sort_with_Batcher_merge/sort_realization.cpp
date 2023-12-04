// Copyright 2023 Prokofev Kirill
#include "task_3/prokofev_k_Shell_sort_with_Batcher_merge/sort_realization.h"

void ShellSortSeq(std::vector<int>& vec) {
  int vecSize = vec.size();
  int k,j,i;
  for (k = vecSize / 2; k > 0; k /= 2) {
    for (i = k; i < vecSize; i++) {
      int t = vec[i];
      for (j = i; j >= k; j -= k) {
        if (t < vec[j - k]) {
          vec[j] = vec[j - k];
        } else {
          break;
        }
      }
      vec[j] = t;
    }
  }
}
std::vector<int> BatcherMerge(const std::vector<int>& vec_1, const std::vector<int>& vec_2)
{
  std::vector<int> resultVector(vec_1.size() + vec_2.size());
  size_t i = 0, j = 0, k = 0;
  while (i < vec_1.size() && j < vec_2.size()) {
    resultVector[k++] = (vec_1[i] < vec_2[j] ? vec_1[i++] : vec_2[j++]);
  }
  while (i < vec_1.size()) {
    resultVector[k++] = vec_1[i++];
  }
  while (j < vec_2.size()) {
    resultVector[k++] = vec_2[j++];
  }
  return resultVector;
}

void ShellSortParallel(std::vector<int>& vec) {
  int numProc;
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  int rankProc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  int mainVecSize = vec.size();
  std::vector<int> preResultVector(vec.size());
  int remainder = vec.size() % numProc;
  int numElemInLocalVector = vec.size() / numProc;
  std::vector<int>localVector(numElemInLocalVector);
  MPI_Scatter(vec.data(), numElemInLocalVector, MPI_INT, localVector.data(),
    numElemInLocalVector, MPI_INT, 0, MPI_COMM_WORLD);
  ShellSortSeq(localVector);
  MPI_Gather(localVector.data(), numElemInLocalVector, MPI_INT,
    preResultVector.data(), numElemInLocalVector, MPI_INT, 0, MPI_COMM_WORLD);
  if (rankProc == 0) {
    std::vector<std::vector<int>> preResultVectorContainer;
    if (remainder > 0) {
      std::vector<int> remainderVect;
      int k = remainder;
      int remIndex = vec.size() - 1;
      while (k != 0) {
        remainderVect.push_back(vec[remIndex]);
        remIndex--;
        k--;
      }
      while (remainder != numElemInLocalVector) 
      {
        remainderVect.push_back(INT_MIN);
        remainder++;
      }
      ShellSortSeq(remainderVect);
      preResultVectorContainer.push_back(remainderVect);
    }
    for (int i = 0; i < numProc; i++) 
    {
      std::vector<int> tmpVect;
      int k = numElemInLocalVector;
      int index = i * numElemInLocalVector;
      while (k != 0) {
        tmpVect.push_back(preResultVector[index]);
        index++;
        k--;
      }
      preResultVectorContainer.push_back(tmpVect);
    }
    vec = preResultVectorContainer[0];
    for (int i = 1; i < preResultVectorContainer.size(); i++)
      vec = BatcherMerge(vec, preResultVectorContainer[i]);
    if (vec.size() != mainVecSize)
      vec.erase(std::remove(vec.begin(),
        vec.end(), INT_MIN), vec.end());
  }
 }

std::vector<int> GenerateRandomVector(int n)
{
  if (n <= 0)
    throw "Error";
  std::random_device dev;
  std::mt19937 generate(dev());
  std::vector<int> newVector(n);
  for (int i = 0; i < n; i++){
    newVector[i] = generate() % n;
  }
  return newVector;
}
