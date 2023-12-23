// Copyright 2023 dmitry-vnn
#include "task_1/vanushkin_d_rect_integration/rectangle_integration.h"
#include <cmath>
#include <algorithm>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

double RectangleIntegrateSequential(
        IntegrateFunction f,
        double lowerBound, double upperBound,
        double step
) {
    double result = 0;

    if (lowerBound >= upperBound) {
        return 0;
    }

    double x0 = lowerBound;
    double x1 = std::min(lowerBound + step, upperBound);

    while (x1 <= upperBound) {
        double middlePoint = (x0 + x1) / 2;

        double height = f(middlePoint);
        double width = x1 - x0;

        result += std::abs(height * width);

        x0 = x1;
        x1 = std::min(x1 + step, upperBound + 1);
    }

    return result;
}


struct Bounds {
    double lowerBound, upperBound;

    explicit Bounds(double lowerBound = 0, double upperBound = 0):
        lowerBound(lowerBound), upperBound(upperBound) {}

    template<class Archive>
    void serialize(Archive& archive, unsigned int) { // NOLINT
        archive & lowerBound;
        archive & upperBound;
    }
};

double RectangleIntegrateParallel(
        IntegrateFunction f,
        double lowerBound,
        double upperBound,
        double step
) {
    boost::mpi::communicator world;

    double localPartitionSum = 0;

    if (world.rank() == 0) {
        auto processAvailable = world.size();

        auto rectangleCount = static_cast<size_t>
                (std::ceil((upperBound - lowerBound) / step));

        auto rectanglesPerProcess = static_cast<size_t>
                (std::ceil((1.0 * rectangleCount) / processAvailable));

        for (size_t i = 1; i < processAvailable; ++i) {
            auto startBound = lowerBound + i * rectanglesPerProcess;

            auto bounds = Bounds(
                    startBound,
                    std::min(startBound + rectanglesPerProcess, upperBound));

            world.send(i, 0, bounds);
        }

        localPartitionSum = RectangleIntegrateSequential(
                f,
                lowerBound,
                std::min(lowerBound + rectanglesPerProcess, upperBound),
                step);
    }

    if (world.rank() != 0) {
        Bounds bounds;
        world.recv(0, 0, bounds);

        localPartitionSum = RectangleIntegrateSequential(
                f, bounds.lowerBound, bounds.upperBound, step);
    }

    double globalSum = 0;

    reduce(world, localPartitionSum, globalSum, std::plus<double>(), 0);

    return globalSum;
}

