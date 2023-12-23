// Copyright 2023 Kashin Stepa
#pragma once
#include <mpi.h>
#include <vector>
#include <iostream>
#include <random>
#include <utility>
#include <algorithm>

std::vector<double> getResult(double* A, int n);

void gaussianEliminationPAR(double* A, int n, int size, int rank);

void generateGaussianMatrix(double* matrix, int n, int max);

void gaussianElimination(double* matrix, int n);
