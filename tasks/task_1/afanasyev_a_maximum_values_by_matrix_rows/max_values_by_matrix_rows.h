// Copyright 2023 Afanasyev Aleksey
#pragma once
#include <mpi.h>
#include <vector>
#include <iostream>
#include <random>
#include <utility>
#include <algorithm>

std::vector<int> CalculateMaxValuesByMatrixRowsSequential(
    const std::vector<int>& matrix,
    size_t rowSize,
    size_t rowCount);

std::vector<int> CalculateMaxValuesByMatrixRowsParallel(
    const std::vector<int>& matrix,
    size_t rowSize,
    size_t rowCount);
