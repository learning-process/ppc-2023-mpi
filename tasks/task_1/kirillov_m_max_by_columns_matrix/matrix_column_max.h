// Copyright 2023 Kirillov Maxim
#pragma once

#include <vector>
#include <iostream>

std::vector<int> getRandomMatrix(int rows, int columns);

std::vector<int> getSequentialMaxInColumns(std::vector<int>matrix, size_t rows, size_t columns);

std::vector<int> getParallelMaxInColumns(std::vector<int>matrix, size_t rows, size_t columns);

