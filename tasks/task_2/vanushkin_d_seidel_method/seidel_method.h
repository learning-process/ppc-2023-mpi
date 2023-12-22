// Copyright 2023 Vanushkin Dmitry

#include <vector>

#pragma once

using DoubleMatrix = std::vector<double>;
using DoubleVector = std::vector<double>;


DoubleVector SequentialSeidelMethod(
        const DoubleMatrix& a,
        const DoubleVector& b,
        double eps
);

DoubleVector ParallelSeidelMethod(
        const DoubleMatrix& a,
        const DoubleVector& b,
        double eps
);
