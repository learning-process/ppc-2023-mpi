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
      sum += abs(matrix[i*n + j]);
    }
    matrix[(n + 1)*i] = sum;

    matrix[n*n + i] = uint_dist(gen);
  }
  return matrix;
};

double* zeidel(double* matrix, int n, double epsilon) {
  boost::mpi::communicator world;
  std::vector<double> tmp_x; int i, j;
  double* local_x = new double[n];
  for (i = 0; i < n; ++i) {
    if (i < n-1) tmp_x.push_back(0);
    local_x[i] = 0;
  }

  int k = 0; bool isReady = false;
  while(!isReady) {
     ++k;

    for (i = 0; i < n; ++i) {
      // res[i] = (1/matrix[i*(n + 1)]) * (matrix[n*n + i] - matrix[1]*tmp_x[(k - 1)*n + 1] - matrix[2]*tmp_x[(k - 1)*n + 2]);
      local_x[i] = (1/matrix[i*(n + 1)]);

      double s = matrix[n*n + i];
      for (j = 0; j < n; ++j) {
        if (i == j) continue;

        // if (world.rank() == 0) {
        //   std::cout << i << ": ";
        //   std::cout << "matrix: " << matrix[i*n + j] << " x: " << local_x[j] << std::endl;
        // }

        if (j < i) s -= matrix[i*n + j]*local_x[j];
        else if (j > i)s -= matrix[i*n + j]*tmp_x[(k - 1)*(n - 1) + (j - 1)];
        else throw -1;
      }
      local_x[i] *= s;

      // if (world.rank() == 0) {
      //   std::cout << "last x: " << tmp_x[(k - 1)*(n - 1) + 1] << std::endl;
      // }

      // if (world.rank() == 0) std::cout << local_x[i] << std::endl;
    }
    // if (world.rank() == 0) {
    //   std::cout << tmp_x.size() << std::endl;
    //   for (i = 0; i < tmp_x.size(); ++i) std::cout << local_x[i] << " ";
    //   std::cout << std::endl << std::endl;
    // }
    // local_b[0] = (1/matrix[0]) * (matrix[n*n + 0] - matrix[1]*tmp_x[(k - 1)*n + 1] - matrix[2]*tmp_x[(k - 1)*n + 2]);
    // local_b[1] = (1/matrix[1]) * (matrix[n*n + 1] - matrix[0]*res[0] - matrix[2]*tmp_x[(k - 1)*n + 2]);
    // local_b[2] = (1/matrix[2]) * (matrix[n*n + 2] - matrix[0]*res[0] - matrix[1]*res[1]);

    double* coefs = new double[n];
    for (i = 0; i < n; ++i) {
      coefs[i] = 0;
      for (j = 0; j < n; ++j) coefs[i] += matrix[i*n + j] * local_x[j];
      coefs[i] = abs(matrix[n*n + i] - coefs[i]);
    }

    double max_coef = coefs[0];
    for (i = 1; i < n; ++i) max_coef = coefs[i] > max_coef ? coefs[i] : max_coef;
    if (max_coef < epsilon) isReady = true;
    // if (world.rank() == 0) std::cout << max_coef << std::endl;

    for (i = 0; !isReady && i < n; ++i) tmp_x.push_back(local_x[i]);
    // if (world.rank() == 0) {
    //   for (i = 0; i < tmp_x.size(); ++i) std::cout << "x = " << tmp_x[i] << std::endl;
    // }
  }

  if (world.rank() == 0) std::cout << "k = " << k << std::endl;
  return local_x;
};
