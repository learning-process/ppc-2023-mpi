// Copyright 2023 Kashin Stepa
#pragma once
#include <mpi.h>
#include <vector>
#include <iostream>
#include <random>
#include <utility>

int difference(const int& a, const int& b);

std::pair<int, int> neighbor_elements(const std::vector<int>& v);

std::pair<int, int> par_neighbor_elements(const std::vector<int>& v);

std::vector<int> create_random_vector(size_t size, unsigned int minDiff, unsigned int maxDiff);
