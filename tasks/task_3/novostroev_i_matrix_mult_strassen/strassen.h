// Copyright 2023 Novostroev Ivan
#pragma once
#include <mpi.h>
#include <vector>
#include <random>

std::vector<double> genVec(int n);

std::vector<double> mult(const std::vector<double>& A,
                         const std::vector<double>& B, int n);

std::vector<double> add(std::vector<double> A, std::vector<double> B);

std::vector<double> sub(const std::vector<double>& A,
 const std::vector<double>& B);

std::vector<double> strassen(const std::vector<double>& A,
 const std::vector<double>& B, std::size_t n);

std::vector<double> strassenMPI(const std::vector<double>& A,
 const std::vector<double>& B,
  std::size_t n, int rank, int size);
