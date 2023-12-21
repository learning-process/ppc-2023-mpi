// Copyright 2023 Smirnov Leonid
#pragma once

#include <mpi.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <functional>

double ApplyGlobalOptimizationSequential(
    std::function<double(double*)> function,
    const double lowerBound, 
    const double upperBound,
    const int numberPartitions,
    const double accuracy
    );

double ApplyGlobalOptimizationParrallel(
    std::function<double(double*)> function,
    const double lowerBound,
    const double upperBound,
    const int numberPartitions,
    const double accuracy
);
