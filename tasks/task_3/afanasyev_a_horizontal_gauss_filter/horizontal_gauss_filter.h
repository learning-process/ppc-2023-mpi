// Copyright 2023 Afanasyev Aleksey
#ifndef TASKS_TASK_3_AFANASYEV_A_HORIZONTAL_GAUSS_FILTER_HORIZONTAL_GAUSS_FILTER_H_
#define TASKS_TASK_3_AFANASYEV_A_HORIZONTAL_GAUSS_FILTER_HORIZONTAL_GAUSS_FILTER_H_

#include <mpi.h>
#include <iostream>
#include <vector>
#include <random>
#include <array>
#include <numeric>
#include <algorithm>
#include <ctime>
#include <list>

const unsigned char matrix_gauss[3][3] {{1, 1, 1}, {1, 9, 1}, {1, 1, 1}};

bool compareVectors(const std::vector<unsigned char>& vec1,
                    const std::vector<unsigned char>& vec2);

std::vector<unsigned char> generateMonoImage(int columns, int rows, int color);

std::vector<unsigned char> generateRandomImage(int columns, int rows);

unsigned char applyGaussianFilterPixelOperation(const std::vector<unsigned char>
                            &image, int x1, int y1, int rows, int cols);

std::vector<unsigned char> applyParGaussianFilter
    (const std::vector<unsigned char>& image, int rows, int cols);

std::vector<unsigned char> applySeqGaussianFilter
    (const std::vector<unsigned char>& image, int rows, int cols);

#endif  // TASKS_TASK_3_AFANASYEV_A_HORIZONTAL_GAUSS_FILTER_HORIZONTAL_GAUSS_FILTER_H_
