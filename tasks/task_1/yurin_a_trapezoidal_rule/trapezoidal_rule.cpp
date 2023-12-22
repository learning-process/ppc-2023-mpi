// Copyright 2023 Nesterov Alexander
#include <utility>
#include <vector>
#include <algorithm>
#include "task_1/yurin_a_trapezoidal_rule/trapezoidal_rule.h"
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

struct Segment {
    double LeftBound, RightBound;

    explicit Segment(double leftBound = 0, double rightBound = 0) {
        LeftBound = leftBound;
        RightBound = rightBound;
    }

    template<class Archive>
    void serialize(Archive& archive, unsigned int) { // NOLINT
        archive & LeftBound;
        archive & RightBound;
    }
};

double TrapezoidArea(
        const std::function<double(const double argument)>& function,
        double firstArgument,
        double secondArgument
        ) {
    double firstValueOfFunction = function(firstArgument);
    double secondValueOfFunction = function(secondArgument);
    double argumentIncrementModule = std::abs(firstArgument-secondArgument);

    return (firstValueOfFunction + secondValueOfFunction) * argumentIncrementModule / 2;
}


double IntegrateWithTrapezoidRuleSequential(
        const std::function<double(const double argument)>& function,
        double leftBound,
        double rightBound,
        double step
        ) {
    double result = 0;
    bool isSignChanges = false;

    if (leftBound >= rightBound) {
        isSignChanges = true;
        std::swap(leftBound, rightBound);
    }

    double firstArgument, secondArgument = leftBound;

    do {
        firstArgument = secondArgument;
        secondArgument = std::min(secondArgument + step, rightBound);
        result += TrapezoidArea(function, firstArgument, secondArgument);
    } while (secondArgument < rightBound);
    return isSignChanges ? (-result) : result;
}

double IntegrateWithTrapezoidRuleParallel(
        const std::function<double(const double argument)>& function,
        double leftBound,
        double rightBound,
        double step
        ) {
    boost::mpi::communicator world;

    bool isSignChanges = false;

    if (leftBound >= rightBound) {
        isSignChanges = true;
        std::swap(leftBound, rightBound);
    }

    std::vector<Segment> segments;
    Segment localSegment;

    if (world.rank() == 0) {
        auto processAvailable = world.size();
        double lengthOfSegment = std::abs((rightBound - leftBound)) / processAvailable;

        for (size_t i = 0; i < processAvailable; i++) {
            double localLeftBound = leftBound + i * lengthOfSegment;
            double localRightBound = std::min(rightBound, localLeftBound + lengthOfSegment);
            Segment segment = Segment(localLeftBound, localRightBound);
            segments.push_back(segment);
        }
    }

    boost::mpi::scatter(world, segments, localSegment, 0);

    double localResult = IntegrateWithTrapezoidRuleSequential(
            function,
            localSegment.LeftBound,
            localSegment.RightBound,
            step);

    double globalResult = 0;
    boost::mpi::reduce(world, localResult, globalResult, std::plus<double>(), 0);

    return isSignChanges ? (-globalResult) : globalResult;
}
