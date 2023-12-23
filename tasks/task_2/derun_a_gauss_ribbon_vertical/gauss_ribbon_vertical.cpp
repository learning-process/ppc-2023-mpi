// Copyright 2023 Derun Andrew

#include "task_2/derun_a_gauss_ribbon_vertical/gauss_ribbon_vertical.h"

#include <mpi.h>

#include <algorithm>
#include <random>

double* getRandomMatrix(int width, int height) {
  if (width < 0 || height < 0) throw -1;
  double* matrix = new double[height * width];
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < height * width; i++) {
    matrix[i] = gen() % 100;
  }
  return matrix;
}

double* getSequentialOperations(double* _matrix, int width, int height) {
  if (height < 0 || width < 0) throw -1;
  double* matrix = new double[height * width];
  for (int i = 0; i < height * width; i++) matrix[i] = _matrix[i];

  for (int i = 0; i < height; i++) {
    for (int j = i + 1; j < height; j++) {
      double coef = matrix[i * width + i] / matrix[j * width + i];
      for (int k = i; k < width; k++)
        matrix[j * width + k] =
            matrix[j * width + k] * coef - matrix[i * width + k];
    }
  }

  double* res = new double[height];
  for (int i = height - 1; i >= 0; i--) {
    double b = matrix[i * width + width - 1];
    for (int j = height - 1; j >= i + 1; j--)
      b -= matrix[i * width + j] * res[j];
    res[i] = b / matrix[i * width + i];
  }
  delete[] matrix;
  return res;
}

double* getParallelOperations(double* matrix, int widht, int height) {
  if (height < 0 || widht < 0) throw -1;
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int local_size = widht / size;
  const int remain = widht % size;
  double* res = 0;
  if (rank < remain)
    res = new double[(local_size + 1) * height];
  else
    res = new double[local_size * height];
  if (rank == 0) {
    for (int proc = size - 1; proc >= 0; proc--) {
      int ind = 0;
      for (int j = proc; j < widht; j += size) {
        for (int i = 0; i < height; i++) {
          res[ind] = matrix[i * widht + j];
          ind++;
        }
      }
      if (proc > 0) {
        MPI_Send(res, ind, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD);
      }
    }
  } else {
    MPI_Status status;
    MPI_Recv(res, (local_size + 1) * height, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD,
             &status);
  }
  double* tmp_widht = new double[height];
  for (int row = 0; row < height; row++) {
    if (row % size == rank) {
      int ind = 0;
      for (int i = height * (row / size); i < height * (row / size + 1); i++) {
        tmp_widht[ind] = res[i];
        ind++;
      }
    }
    MPI_Bcast(tmp_widht, height, MPI_DOUBLE, row % size, MPI_COMM_WORLD);
    int count;
    if (rank < remain)
      count = local_size + 1;
    else
      count = local_size;
    for (int j = row / size; j < count; j++) {
      double tmp = res[j * height + row];
      for (int k = 0; k < height; k++) {
        if (k == row) {
          res[j * height + k] /= tmp_widht[k];
        } else {
          res[j * height + k] -= tmp * tmp_widht[k] / tmp_widht[row];
        }
      }
    }
  }
  if (rank == (widht - 1) % size) {
    MPI_Request req;
    MPI_Isend(res + ((widht - 1) / size) * height, height, MPI_DOUBLE, 0, 0,
              MPI_COMM_WORLD, &req);
  }
  if (rank == 0) {
    res = new double[height];
    MPI_Status status;
    MPI_Recv(res, height, MPI_DOUBLE, (widht - 1) % size, 0, MPI_COMM_WORLD,
             &status);
  }
  return res;
}
