// Copyright 2023 Ulyanov Daniil

#include "task_1/ulyanov_d_matrix_sum/matrix_sum.h"

int sequentialSum(const std::vector<int> &v)
{
  int sum = 0;

  for (int i = 0; i < v.size(); i++)
  {
    sum += v[i];
  }

  return sum;
}

int parallelSum(const std::vector<int> &v)
{
  int num, rank;

  MPI_Comm_size(MPI_COMM_WORLD, &num);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int block = v.size() / num, remain = v.size() % num;
  std::vector<int> counts(num), indexs(num);

  for (int i = 0; i < num; i++)
  {
    if (i < remain)
      counts[i] = block + 1;
    else
      counts[i] = block;
    if (i > 0)
      indexs[i] = (indexs[i - 1] + counts[i - 1]);
    else
      indexs[i] = 0;
  }

  std::vector<int> localVec(counts[rank]);

  MPI_Scatterv(v.data(), counts.data(), indexs.data(), MPI_INT, localVec.data(), counts[rank], MPI_INT, 0, MPI_COMM_WORLD);

  int localSum = sequentialSum(localVec), globalSum = 0;

  MPI_Reduce(&localSum, &globalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return globalSum;
}