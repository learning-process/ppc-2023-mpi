// Copyright 2023 Shmelev Ivan
#ifndef TASKS_TASK_3_SHMELEV_I_HORIZONTAL_GAUSS_FILTER_HORIZONTAL_GAUSS_FILTER_H_
#define TASKS_TASK_3_SHMELEV_I_HORIZONTAL_GAUSS_FILTER_HORIZONTAL_GAUSS_FILTER_H_

#include <mpi.h>
#include <iostream>
#include <vector>
#include <random>
#include <array>
#include <numeric>
#include <algorithm>
#include <ctime>
#include <list>

bool compare_vectors(const std::vector<unsigned char>& vec1,
                    const std::vector<unsigned char>& vec2);
const unsigned char matrix_gauss[3][3] {{1, 4, 1}, {4, 12, 4}, {1, 4, 1}};
std::vector<unsigned char> generate_random_image(int columns, int rows);
unsigned char gaussian_filter_pixel_operation(const std::vector<unsigned char>
                                    &image, int x1, int y1, int rows, int cols);
std::vector<unsigned char> par_gaussian_filter(std::vector<unsigned char> image,
                                                            int rows, int cols);
std::vector<unsigned char> seq_gaussian_filter(std::vector<unsigned char> image,
                                                            int rows, int cols);

#endif  // TASKS_TASK_3_SHMELEV_I_HORIZONTAL_GAUSS_FILTER_HORIZONTAL_GAUSS_FILTER_H_
