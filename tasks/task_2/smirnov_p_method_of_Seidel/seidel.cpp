// Copyright 2023 Smirnov Pavel
#include <mpi.h>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "task_2/smirnov_p_method_of_Seidel/seidel.h"

std::vector<double> randMatrix(int n, int type) {
  if (n < 0) {
    throw - 2;
  }

  std::srand(time(NULL));
  std::vector<double> result;

  if (type == TYPE_A) {
    for (int i = 0; i < n; i++) {
      int sum = 0;
      for (int j = 0; j < n; j++) {
        if (i == j) {
          result.push_back(0);
        } else {
          int temp = std::rand() % 50;
          sum += temp;
          result.push_back(temp);
        }
      }

      result[i * n + i] = sum + 1;
    }
  } else if (type == TYPE_B) {
    for (int i = 0; i < n; i++) {
      result.push_back(std::rand() % 50);
    }
  }

  return result;
}

std::vector<double> seidel_solve(std::vector<double> A, std::vector<double> B,
                                 int n, double eps) {
  if (n < 0) {
    throw - 2;
  }

  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int delta = n / size;
  int leftBound = delta * rank;
  int rightBound = delta * (rank + 1);

  if (rank == size - 1 && delta != 0) {
    rightBound = n;
  }
  if (rank == 0 && delta == 0) {
    rightBound = n;
  }

  double localSum = 0.0, globalSum = 0.0, norm = 0.0;

  std::vector<double> x(n);
  std::vector<double> p(n);

  do {
    for (int i = 0; i < n; i++) {
      p[i] = x[i];
    }

    for (int i = 0; i < n; i++) {
      localSum = 0.0;
      globalSum = 0.0;

      for (int j = leftBound; j < rightBound; j++) {
        if ((0 <= j) && (j < i)) {
          localSum += (A[i * n + j] * x[j]);
        } else if ((i + 1 <= j) && (j < n)) {
          localSum += (A[i * n + j] * p[j]);
        }
      }

      MPI_Allreduce(&localSum, &globalSum, 1, MPI_DOUBLE, MPI_SUM,
                    MPI_COMM_WORLD);

      x[i] = (B[i] - globalSum) / A[i * n + i];
    }

    double local_norm = 0.0;
    norm = 0.0;
    for (int i = leftBound; i < rightBound; i++) {
      local_norm += std::pow(x[i] - p[i], 2);
    }
    MPI_Allreduce(&local_norm, &norm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  } while (!(std::sqrt(norm) < eps));

  return x;
}

std::vector<double> seidel_solve_s(std::vector<double> A, std::vector<double> B,
                                   int n, double eps) {
  if (n < 0) {
    throw - 2;
  }

  std::vector<double> x(n);
  std::vector<double> p(n);

  double norm = 0;
  do {
    for (int i = 0; i < n; i++) {
      p[i] = x[i];
    }

    for (int i = 0; i < n; i++) {
      double var = 0;

      for (int j = 0; j < i; j++) {
        var += (A[i * n + j] * x[j]);
      }

      for (int j = i + 1; j < n; j++) {
        var += (A[i * n + j] * p[j]);
      }

      x[i] = (B[i] - var) / A[i * n + i];
    }

    norm = 0;
    for (int i = 0; i < n; i++) {
      norm += std::pow(x[i] - p[i], 2);
    }
  } while (!(std::sqrt(norm) < eps));

  return x;
}
