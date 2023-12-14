// Copyright 2023 Smirnova Daria
#ifndef MODULES_TASK_3_SMIRNOVA_D_MONTE_CARLO_MULTIPLE_INTEGRATION_MONTE_CARLO_MULTIPLE_INTEGRATION_H_
#define MODULES_TASK_3_SMIRNOVA_D_MONTE_CARLO_MULTIPLE_INTEGRATION_MONTE_CARLO_MULTIPLE_INTEGRATION_H_

#include <mpi.h>
#include <vector>

double Monte_Carlo_Multiple_Integration(double(*f)(std::vector<double>), const std::vector<double>& a,
  const std::vector<double>& b, int n);

double Monte_Carlo_Multiple_Integration_Parallel(double(*f)(std::vector<double>), const std::vector<double>& a,
  const std::vector<double>& b, int n);

#endif  // MODULES_TASK_3_SMIRNOVA_D_MONTE_CARLO_MULTIPLE_INTEGRATION_MONTE_CARLO_MULTIPLE_INTEGRATION_H_
