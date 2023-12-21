// Copyright 2023 Smirnov Leonid

#include <task_3/smirnov_l_search_area/search_area.h>
#include <mpi.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <functional>
#include <utility>

double ApplyGlobalOptimizationSequential(
    std::function<double(double*)> function,
    const double lowerBound,
    const double upperBound,
    const int numberPartitions,
    const double accuracy) {
    std::vector<double> interimResults;
    interimResults.push_back(lowerBound);
    interimResults.push_back(upperBound);

    int maxContr = 1;
    double coef = 2, controlParameter;

    double meanRateOfChange =
        std::abs(
            function(&interimResults.at(1)) -
            function(&interimResults.at(0)))
        / (interimResults.at(1) - interimResults.at(0));

    double stepSize = (meanRateOfChange > 0) ? meanRateOfChange * coef : 1;

    double candidatePoint;
    candidatePoint = ((interimResults.at(1) + interimResults.at(0)) / 2 -
        (function(&interimResults.at(1)) - function(&interimResults.at(0)))
        / (2 * stepSize));
    interimResults.push_back(candidatePoint);

    int counter = interimResults.size() - 1;

    while (counter < numberPartitions) {
        std::sort(interimResults.begin(),
            interimResults.begin() + interimResults.size());

        meanRateOfChange = std::abs(
            function(&interimResults.at(1)) -
            function(&interimResults.at(0)))
            / (interimResults.at(1) - interimResults.at(0));
        
        for (int i = 2; i <= counter; i++) {
            meanRateOfChange = std::max(
                meanRateOfChange,
                std::abs(
                    function(&interimResults.at(i)) -
                    function(&interimResults.at(i - 1)))
                / (interimResults.at(i) - interimResults.at(i - 1)));
        }

        stepSize = (meanRateOfChange > 0) ? meanRateOfChange * coef : 1;

        controlParameter =
            stepSize * (interimResults.at(1) - interimResults.at(0)) +
            std::pow((function(&interimResults.at(1)) -
                function(&interimResults.at(0))), 2)
            / (stepSize * (interimResults.at(1) - interimResults.at(0))) -
            2 * (function(&interimResults.at(1))
                + function(&interimResults.at(0)));

        maxContr = 1;

        for (int i = 2; i <= counter; ++i) {
            double currentContr =
                stepSize * (interimResults.at(i) - interimResults.at(i - 1)) +
                std::pow((function(&interimResults.at(i))
                    - function(&interimResults.at(i - 1))), 2)
                / (stepSize * (interimResults.at(i)
                    - interimResults.at(i - 1))) -
                2 * (function(&interimResults.at(i))
                    + function(&interimResults.at(i - 1)));

            if (controlParameter < currentContr) {
                controlParameter = currentContr;
                maxContr = i;
            }
        }

        candidatePoint = ((interimResults.at(maxContr)
            + interimResults.at(maxContr - 1)) / 2 -
            (function(&interimResults.at(maxContr))
                - function(&interimResults.at(maxContr - 1)))
            / (2 * stepSize));

        interimResults.push_back(candidatePoint);
        counter++;

        if (interimResults.at(maxContr)
            - interimResults.at(maxContr - 1) < accuracy) {
            break;
        }
    }

    return interimResults.at(maxContr);
}

double ApplyGlobalOptimizationParrallel(
    std::function<double(double*)> function,
    const double lowerBound,
    const double upperBound,
    const int numberPartitions,
    const double accuracy) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1) {
        return ApplyGlobalOptimizationSequential(
            function,
            lowerBound,
            upperBound,
            numberPartitions,
            accuracy);
    }

    std::vector<double> interimResults(size);
    double lower = lowerBound + ((upperBound - lowerBound) / size) * rank;
    double upper = lower + (upperBound - lowerBound) / size;

    double localRes = ApplyGlobalOptimizationSequential(
        function, lower, upper, numberPartitions, accuracy);

    MPI_Gather(&localRes, 1, MPI_DOUBLE, &interimResults.at(0),
        1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double bestValue = function(&interimResults.at(0));
        for (int i = 1; i < size; ++i) {
            double currentValue = function(&interimResults.at(0) + i);
            if (currentValue < bestValue) {
                std::swap(interimResults.at(i), interimResults.at(0));
                bestValue = currentValue;
            }
        }
    }
    return interimResults.at(0);
}
