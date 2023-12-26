// Copyright 2023 Ionova Ekaterina
#include "task_1/ionova_e_int_rect/int_rect.h"

bool isEqual(double valueOne, double valueTwo, double epsilon) {
  return abs(valueOne - valueTwo) <= epsilon;
}

double seqIntegrationRectangle(double a, double b, double n,
                               std::function<double(double)> func) {
  double h = (b - a) / n;
  double sum = 0.0;

  for (int i = 0; i < n; ++i) sum += func(a + i * h);
  return sum * h;
}

double parIntegrationRectangle(double a, double b, uint32_t n,
                               std::function<double(double)> func,
                               MPI_Comm comm) {
  int rank = 0;
  int sizeWorld = 0;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &sizeWorld);

  double len = b - a;
  double chunk = len / sizeWorld;

  double localA = a + rank * chunk;
  double localB = localA + chunk;
  double localH = (localB - localA) / n;
  double localSum = 0.0;
  double globalSum = 0.0;

  for (int i = 0; i < n; ++i) localSum += func(localA + i * localH);
  localSum *= localH;

  MPI_Allreduce(&localSum, &globalSum, 1, MPI_DOUBLE, MPI_SUM, comm);

  return globalSum;
}
