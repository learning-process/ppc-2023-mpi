// Copyright 2023 Shishkina Valeria
#include "task_3/shishkina_l_metodSimpsona/metodSimpsona.h"

double f(double x, double y) { return x * y; }

double parallelSimpson(double a, double b, double c, double d, int n) {
  MPI_Comm comm = MPI_COMM_WORLD;

  int num_procs;
  MPI_Comm_size(comm, &num_procs);
  int rank;
  MPI_Comm_rank(comm, &rank);

  if (n % 2 != 0) n++;
  int local_n = n / num_procs;
  int remain = n % num_procs;
  if (rank == 0 && remain != 0) local_n += remain;
  double h1 = (b - a) / (n);
  double h2 = (d - c) / (n);
  double local_sum = 0.0;
  double coef1 = 0.0;
  double coef2 = 0.0;
  /*int start_index = rank * local_n;
  int end_index = start_index + local_n;*/
  int start_index = rank * local_n + (rank > remain ? remain : rank);
  int end_index = start_index + local_n - 1;
  for (int i = start_index; i <= end_index; i++) {
    double x = a + i * h1;
    for (int j = 0; j <= n; j++) {
      double y = c + j * h2;

      if (i == 0 || i == n)
        coef1 = 1.0;
      else {
        if (i % 2 == 0)
          coef1 = 2.0;
        else
          coef1 = 4.0;
      }

      if (j == 0 || j == n)
        coef2 = 1.0;
      else {
        if (j % 2 == 0)
          coef2 = 2.0;
        else
          coef2 = 4.0;
      }

      coef1 = coef1 * coef2;
      local_sum += coef1 * f(x, y);
    }
  }

  double global_sum;
  MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, comm);

  if (rank == 0) {
    return (h1 * h2) / (9.0) * global_sum;
  } else {
    return 0.0;
  }
}
double simpson(double a, double b, double c, double d, int n) {
  if (n % 2 != 0) n++;
  double h1 = (b - a) / (n);
  double h2 = (d - c) / (n);
  double sum = 0.0;
  double coef1 = 0.0;
  double coef2 = 0.0;

  for (int i = 0; i <= n; i++) {
    double x = a + i * h1;
    for (int j = 0; j <= n; j++) {
      double y = c + j * h2;
      if (i == 0 || i == n)
        coef1 = 1.0;
      else {
        if (i % 2 == 0)
          coef1 = 2.0;
        else
          coef1 = 4.0;
      }
      if (j == 0 || j == n)
        coef2 = 1.0;
      else {
        if (j % 2 == 0)
          coef2 = 2.0;
        else
          coef2 = 4.0;
      }
      coef1 = coef1 * coef2;
      sum += coef1 * f(x, y);
    }
  }

  return (h1 * h2 / 9.0) * sum;
}
