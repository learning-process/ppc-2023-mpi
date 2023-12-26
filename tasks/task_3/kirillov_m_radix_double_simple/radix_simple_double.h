// Copyright 2023 Kirillov Maxim
#pragma once

#include <vector>

std::vector<double> getRandomVector(int size, double min, double max);
std::vector<double> getSeqRadixSortSimpleMerge(const std::vector<double>& vec);
std::vector<double> getParRadixSortSimpleMerge(const std::vector<double>& vec);
std::vector<double> simpleMerge(const std::vector<double>& first,
                                const std::vector<double>& second);
void countSort(double* in, int digit, int size);
