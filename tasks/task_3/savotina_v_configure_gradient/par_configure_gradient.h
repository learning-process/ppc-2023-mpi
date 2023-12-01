// Copyright 2023 Savotina Valeria
#ifndef TASKS_TASK_3_SAVOTINA_V_CONFIGURE_GRADIENT_PAR_CONFIGURE_GRADIENT_H_
#define TASKS_TASK_3_SAVOTINA_V_CONFIGURE_GRADIENT_PAR_CONFIGURE_GRADIENT_H_

#include <mpi.h>

#include <numeric>
#include <stdexcept>
#include <vector>

#include "task_3/savotina_v_configure_gradient/overload_operators.h"

std::vector<double> multMatrixOnVectorChunks(size_t mMatr, size_t n,
                                             const std::vector<double>& matr,
                                             const std::vector<double>& vec);
std::vector<double> parMultMatrixOnVector(size_t n,
                                          const std::vector<double>& matr,
                                          const std::vector<double>& vec,
                                          MPI_Comm comm);
double parScalarProductVectors(const std::vector<double>& vecOne,
                               const std::vector<double>& vecTwo,
                               MPI_Comm comm);
std::vector<double> parConfigureGradientMethod(size_t n,
                                               const std::vector<double>& A,
                                               const std::vector<double>& b,
                                               MPI_Comm comm, double tol = 1e-2,
                                               size_t countIter = 10000);

#endif  // TASKS_TASK_3_SAVOTINA_V_CONFIGURE_GRADIENT_PAR_CONFIGURE_GRADIENT_H_
