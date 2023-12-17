// Copyright 2023 Khodyrev Fedor
#ifndef TASKS_TASK_2_KHODYREV_F_CONTRAST_ENHANCEMENT_METHOD_CONTRAST_ENHANCEMENT_H_
#define TASKS_TASK_2_KHODYREV_F_CONTRAST_ENHANCEMENT_METHOD_CONTRAST_ENHANCEMENT_H_

#include <mpi.h>
#include <vector>

bool compareMatrix(const std::vector<std::vector<int>>& v1,
    const std::vector<std::vector<int>>& v2);
std::vector<std::vector<int>> generate_image(int rows, int cols);
std::vector<std::vector<int>> seq_linear_contrast_stretching
    (std::vector<std::vector<int>> image, int rows, int cols);
std::vector<std::vector<int>> par_linear_contrast_stretching
    (std::vector<std::vector<int>> image, int rows, int cols);


#endif  // TASKS_TASK_2_KHODYREV_F_CONTRAST_ENHANCEMENT_METHOD_CONTRAST_ENHANCEMENT_H_
