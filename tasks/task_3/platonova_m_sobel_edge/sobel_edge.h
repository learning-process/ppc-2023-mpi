// Copyright 2023 Platonova Maria
#ifndef TASKS_TASK_3_PLATONOVA_M_SOBEL_EDGE_SOBEL_EDGE_H_
#define TASKS_TASK_3_PLATONOVA_M_SOBEL_EDGE_SOBEL_EDGE_H_

#pragma once

#include <cmath>
#include <vector>

using ImageMatrix = std::vector<int>;

ImageMatrix SequentialSobelOperator(
        const ImageMatrix& image, size_t width, size_t height);

ImageMatrix ParallelSobelOperator(
        const ImageMatrix& image, size_t width, size_t height);

#endif  // TASKS_TASK_3_PLATONOVA_M_SOBEL_EDGE_SOBEL_EDGE_H_
