// Copyright 2023 Novostroev Ivan
#pragma once
#include <mpi.h>
#include <vector>
#include <iostream>
#include <utility>

std::pair<std::vector<int>, std::vector<int>> distribution
(bool flag, const size_t row, const size_t col);

std::vector<double> transposition
(const std::vector<double>& matrix, const size_t row, const size_t col);

std::vector<double> sumMatrixColEquential
(const std::vector<double>& matrix, const size_t row, const size_t col);

std::vector<double> sumMatrixColParallel
(const std::vector<double>& matrix, const size_t row, const size_t col);

std::pair<int, int> countAndOffsetDistribution
(const size_t row, int rank);
