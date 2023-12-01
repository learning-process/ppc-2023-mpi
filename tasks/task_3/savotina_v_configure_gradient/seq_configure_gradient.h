// Copyright 2023 Savotina Valeria
#ifndef TASKS_TASK_3_SAVOTINA_V_CONFIGURE_GRADIENT_SEQ_CONFIGURE_GRADIENT_H_
#define TASKS_TASK_3_SAVOTINA_V_CONFIGURE_GRADIENT_SEQ_CONFIGURE_GRADIENT_H_

#include <cmath>
#include <numeric>
#include <stdexcept>
#include <vector>

#include "task_3/savotina_v_configure_gradient/overload_operators.h"

double seqScalarProductVectors(const std::vector<double>& vecOne,
                               const std::vector<double>& vecTwo);
std::vector<double> seqMultMatrixOnVector(size_t n,
                                          const std::vector<double>& matr,
                                          const std::vector<double>& vec);
std::vector<double> seqConfigureGradientMethod(size_t n,
                                               const std::vector<double>& A,
                                               const std::vector<double>& b,
                                               double tol = 1e-2,
                                               size_t countIter = 10000);

bool isEqualVectors(const std::vector<double>& vecOne,
                    const std::vector<double>& vecTwo, double epsilon = 1e-3);

#endif  // TASKS_TASK_3_SAVOTINA_V_CONFIGURE_GRADIENT_SEQ_CONFIGURE_GRADIENT_H_
