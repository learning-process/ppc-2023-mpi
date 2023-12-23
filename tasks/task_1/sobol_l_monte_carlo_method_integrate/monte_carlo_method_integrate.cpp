// Copyright 2023 Sobol Lubov

#include <mpi.h>
#include <random>
#include <ctime>
#include <algorithm>
#include "task_1/sobol_l_monte_carlo_method_integrate/monte_carlo_method_integrate.h"


double calculateMonteCarloIntegral(double lowerBound, double upperBound,
                                   double(*targetFunction)(double), int numberOfPoints) {
    int processCount, currentProcess;
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);

    std::mt19937 randomGen;
    time_t currentTime = static_cast<time_t>(currentProcess);
    randomGen.seed(static_cast<unsigned int>(time(&currentTime)));
    std::uniform_real_distribution<> uniformDistribution(0, 1);

    double interval = (upperBound - lowerBound) / processCount;
    int localPointCount = numberOfPoints / processCount;
    double localSum = 0.0;
    double globalSum = 0.0;
    for (int i = 0; i < localPointCount; i++) {
        localSum += targetFunction(lowerBound + interval * currentProcess +
                                   uniformDistribution(randomGen) * interval);
    }
    localSum *= interval / localPointCount;

    MPI_Reduce(&localSum, &globalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return globalSum;
}
