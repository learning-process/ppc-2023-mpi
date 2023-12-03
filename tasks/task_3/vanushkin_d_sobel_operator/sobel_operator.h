// Copyright 2023 Vanushkin Dmitry

#pragma once

#include <cmath>
#include <vector>

using ImageMatrix = std::vector<int>;

ImageMatrix SequentialSobelOperator(const ImageMatrix& image, size_t width, size_t height);
ImageMatrix ParallelSobelOperator(const ImageMatrix& image, size_t width, size_t height);
