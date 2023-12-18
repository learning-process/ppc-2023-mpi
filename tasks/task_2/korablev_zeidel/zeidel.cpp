// Copyright 2023 Korablev Nikita
#include "task_2/korablev_zeidel/zeidel.h"

double* getMatrix(int n) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_int_distribution<int> uint_dist(-100, 100);

  double* matrix = new double[n*(n + 1)];
  int i, j;
  for (i = 0; i < n; ++i) {
    double sum = 0;
    for (j = 0; j < n; ++j) {
      if (i == j) continue;
      matrix[i*n + j] = uint_dist(gen);
      sum += fabs(matrix[i*n + j]);
    }
    matrix[(n + 1)*i] = sum;

    matrix[n*n + i] = uint_dist(gen);
  }
  return matrix;
};

double* zeidel(double* matrix, int n, double epsilon) {
  boost::mpi::communicator world;
  int i, j;
  double* tmp_x = new double[n]; 
  double* local_x = new double[n];
  for (i = 0; i < n; ++i) {
    tmp_x[0] = 0;
    local_x[i] = 0;
  }

  int k = 0;
  double* coefs = new double[n];
  while(true) {
    ++k;
    for (i = 0; i < n; ++i) {
      local_x[i] = (1/matrix[i*(n + 1)]);

      double s = matrix[n*n + i];
      for (j = 0; j < n; ++j) {
        if (i == j) continue;
        if (j < i) s -= matrix[i*n + j]*local_x[j];
        else if (j > i)s -= matrix[i*n + j]*tmp_x[j];
        else throw -1;
      }
      local_x[i] *= s;
    }

    for (i = 0; i < n; ++i) coefs[i] = fabs(local_x[i] - tmp_x[i]);

    double max_coef = coefs[0];
    for (i = 1; i < n; ++i) max_coef = coefs[i] > max_coef ? coefs[i] : max_coef;
    if (max_coef < epsilon) break;
    else {
      for (i = 0; i < n; ++i) tmp_x[i] = local_x[i];
      if (k > 100) throw -1;
    }
  }
  delete[] tmp_x;
  delete[] coefs;
  
  if (world.rank() == 0) std::cout << "k = " << k << std::endl;
  return local_x;
};
