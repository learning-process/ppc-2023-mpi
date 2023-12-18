// Copyright 2023 Korablev Nikita
#include "task_2/korablev_zeidel/zeidel.h"

double* zeidel(double* matrix, int n, double epsilon) {
  boost::mpi::communicator world;
  int i, j;
  double* tmp_x = new double[n](); 
  double* local_x = new double[n]();
  double* coefs = new double[n];

  int k = 0;
  while(true) {
    ++k;
    for (i = 0; i < n; ++i) {
      local_x[i] = (1/matrix[i*(n + 1)]);

      double s = matrix[n*n + i];
      for (j = 0; j < n; ++j) {
        if (i == j) continue;
        if (j < i) s -= matrix[i*n + j]*local_x[j];
        else if (j > i) s -= matrix[i*n + j]*tmp_x[j];
        else { std::cout << "Error: new catch" << std::endl; throw -1; }
      }
      local_x[i] *= s;
    }

    for (i = 0; i < n; ++i) coefs[i] = fabs(local_x[i] - tmp_x[i]);

    double max_coef = coefs[0];
    for (i = 1; i < n; ++i) max_coef = coefs[i] > max_coef ? coefs[i] : max_coef;
    // for (i = 0; i < n; ++i) std ::cout << tmp_x[i] << " ";
    // std::cout << std::endl;
    if (max_coef < epsilon) break;
    else {
      if (k > 100 && world.rank() == 0) {
        std::cout << "Error: k = " << k << std::endl;
        for (i = 0; i < n; ++i) std ::cout << tmp_x[i] << " ";
        std::cout << std::endl;
        throw -1;
      }
      for (i = 0; i < n; ++i) tmp_x[i] = local_x[i];
    }
  }
  delete[] tmp_x;
  delete[] coefs;
  
  return local_x;
};

double* parallel_zeidel(double* matrix, int n, double epsilon) {
  boost::mpi::communicator world;

  int *ProcSizes = new int[world.size()];
  for (int i = 0; i < world.size(); i++) {
    ProcSizes[i] = n / world.size() + (i < n % world.size() ? 1 : 0);
  }

  // double *rows = new double[ProcSizes[world.rank()] * (n + 1)]();

  int i, j;
  double* tmp_x = new double[n](); 
  double* local_x = new double[n]();
  double* coefs = new double[n]();

  int k = 0;
  while(true) {
    ++k;

    int proc_num = world.rank();
    int len = ProcSizes[proc_num];
    int local_start = world.rank() * len;
    int local_end = local_start + len;

    for (i = 1; i < proc_num; ++i) {
      if (world.rank() == i) broadcast(world, local_x, n, i-1);
    }

    for (i = local_start; i < local_end && i != n; ++i) {
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

    for (i = 0; i < proc_num-1; ++i) {
      if (world.rank() == i) {
        broadcast(world, local_x, n, i);
      }
    }

    if (world.rank() == world.size() - 1) broadcast(world, local_x, n, world.rank());
    else broadcast(world, local_x, n, world.rank());

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
  
  return local_x;
}

