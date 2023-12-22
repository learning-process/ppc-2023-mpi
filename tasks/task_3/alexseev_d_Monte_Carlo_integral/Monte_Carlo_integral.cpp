// Copyright 2023 Alexseev Danila

#include "task_3/alexseev_d_Monte_Carlo_integral/Monte_Carlo_integral.h"

double func1VarPow(const std::vector<double>& x) {
    return std::pow(x[0], 2);
}

double func1VarCos(const std::vector<double>& x) {
    return std::cos(-x[0]);
}

double func2VarExp(const std::vector<double>& x) {
    return std::exp(-x[0] - x[1]);
}

double func2VarSin(const std::vector<double>& x) {
    return std::sin(-x[0]) - x[1];
}

double func2VarSum(const std::vector<double>& x) {
    return -x[0] + x[1];
}

double parallelCalculateIntegral(func foo, const int dimensionOfSpace,
    const int pointsCount, const std::vector<double>& lowerLimits, const std::vector<double>& upperLimits) {
    double integral = 0.0, globalIntegral, res = 0.0;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int i = rank; i < pointsCount; i += size) {
        std::vector<double> x;
        for (int j = 0; j < dimensionOfSpace; ++j) {
            x.push_back(lowerLimits[j] + (upperLimits[j] - lowerLimits[j]) * dist(gen));
        }
        integral += foo(x);
    }

    MPI_Reduce(&integral, &globalIntegral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double spaceVolume = 1.0;
        for (int j = 0; j < dimensionOfSpace; ++j) {
            spaceVolume *= (upperLimits[j] - lowerLimits[j]);
        }
        double average = globalIntegral / static_cast<double>(pointsCount);
        res = spaceVolume * average;
    }
    return res;
}
