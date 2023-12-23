// Copyright 2023 dmitry-vnn

#pragma once

#include <iostream>
#include <functional>

using IntegrateFunction = const std::function<double(double)>&;

double RectangleIntegrateSequential(
        IntegrateFunction f,
        double lowerBound, double upperBound,
        double step
);

double RectangleIntegrateParallel(
        IntegrateFunction f,
        double lowerBound, double upperBound,
        double step
);
