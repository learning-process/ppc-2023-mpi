// Copyright 2023 Filatov Maxim

#include "task_3/filatov_m_calc_integral_rec/calc_integral_rec.h"

#include <functional>
#include <limits>
#include <algorithm>

struct RandomNumber {
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist;
    RandomNumber() {
        gen = std::mt19937(rd());
        dist = std::uniform_int_distribution<int>(
            DEFAULT_RANDOM_MIN, DEFAULT_RANDOM_MAX);
    }
    RandomNumber(int min, int max) {
        gen = std::mt19937(rd());
        dist = std::uniform_int_distribution<int>(min, max);
    }
    int operator()() {
        return dist(gen);
    }
};

std::pair<double, double> getRandomLimits(int min, int max) {
    RandomNumber randomNumber(min, max);
    std::pair<double, double> pair(NULL, NULL);
    pair.first = randomNumber();
    pair.second = randomNumber();

    return std::make_pair(
        std::min(pair.first, pair.second),
        std::max(pair.first, pair.second));
}

int getAreasNumber(const std::vector<int>& partitions_number) {
    return std::accumulate(
        partitions_number.begin(),
        partitions_number.end(),
        1,
        std::multiplies<int>());
}

double calculateVolume(
    const std::vector<int>& partitions_number,
    const std::vector<std::pair<double, double>>& integration_limits) {
    return std::inner_product(
        partitions_number.begin(), partitions_number.end(),
        integration_limits.begin(), 1.0,
        std::multiplies<double>(),
        [](int partitions, const std::pair<double, double>& limits) {
            return (limits.second - limits.first) /
                static_cast<double>(partitions);
        });
}

void updateStepsAndVariables(
    int area_num,
    std::vector<double>* variables,
    std::vector<double>* steps,
    const std::vector<int>& partitions_number,
    const std::vector<std::pair<double, double>>& integration_limits) {
    if (variables != nullptr && steps != nullptr) {
        int remaining_index = area_num;
        for (int i = 0; i < variables->size(); ++i) {
            double interval =
                integration_limits[i].second - integration_limits[i].first;
            double step = interval / static_cast<double>(partitions_number[i]);
            (*steps)[i] = step;
            int num_steps = (i == variables->size() - 1) ?
                remaining_index : remaining_index / partitions_number[i];
            remaining_index = (i == variables->size() - 1) ?
                0 : remaining_index % partitions_number[i];
            (*variables)[i] =
                integration_limits[i].first + (num_steps + 0.5) * step;
        }
    }
}

double calculateIntegralRectangles(
    int first_area_num,
    int last_area_num,
    int variables_number,
    double integrand(const std::vector<double>&),
    const std::vector<int>& partitions_number,
    const std::vector<std::pair<double, double>>& integration_limits) {
    double result = 0.0;
    double volume = calculateVolume(partitions_number, integration_limits);

    for (int area_num = first_area_num; area_num < last_area_num; area_num++) {
        std::vector<double> variables(variables_number);
        std::vector<double> steps(variables_number);

        updateStepsAndVariables(
            area_num,
            &variables,
            &steps,
            partitions_number,
            integration_limits);

        result += integrand(variables) * volume;
    }

    return result;
}


double integrateSequentially(
    int variables_number,
    double integrand(const std::vector<double>&),
    const std::vector<int>& partitions_number,
    const std::vector<std::pair<double, double>>& integration_limits) {
    return calculateIntegralRectangles(
        0,
        getAreasNumber(partitions_number),
        variables_number, integrand,
        partitions_number, integration_limits);
}

void initCommunicatorAndBroadcastLimits(
    const boost::mpi::communicator& comm,
    std::vector<std::pair<double, double>>* integration_limits,
    int variables_number) {
    if (integration_limits != nullptr) {
        integration_limits->resize(variables_number);
        boost::mpi::broadcast(
            comm,
            integration_limits->data(),
            variables_number,
            0);
    }
}

std::pair<int, int> getLocalAreaBounds(
    const boost::mpi::communicator& comm,
    int global_areas_number,
    int local_areas_number) {
    int first_area = local_areas_number * comm.rank();
    int last_area = first_area + local_areas_number;
    if (comm.rank() == comm.size() - 1) {
        last_area += global_areas_number % comm.size();
    }
    return std::make_pair(first_area, last_area);
}

double computeLocalResult(
    int first_area,
    int last_area,
    int variables_number,
    double integrand(const std::vector<double>&),
    const std::vector<int>& partitions_number,
    const std::vector<std::pair<double, double>>& integration_limits) {
    std::vector<double> variables(variables_number);
    return calculateIntegralRectangles(
        first_area,
        last_area,
        variables_number,
        integrand, partitions_number, integration_limits);
}

double integrateInParallel(
    int variables_number,
    double integrand(const std::vector<double>&),
    const std::vector<int>& partitions_number,
    std::vector<std::pair<double, double>> integration_limits) {
    boost::mpi::communicator comm;
    initCommunicatorAndBroadcastLimits(
        comm,
        &integration_limits,
        variables_number);
    int global_areas_number = getAreasNumber(partitions_number);
    int local_areas_number = global_areas_number / comm.size();
    auto [first_area, last_area] =
        getLocalAreaBounds(comm, global_areas_number, local_areas_number);
    double local_result =
        computeLocalResult(
            first_area,
            last_area,
            variables_number,
            integrand, partitions_number,
            integration_limits);
    double global_result = 0;
    boost::mpi::reduce(
        comm,
        local_result,
        global_result,
        std::plus<double>(),
        0);
    if (comm.rank() == 0) {
        return global_result;
    } else {
        return std::numeric_limits<double>::quiet_NaN();
    }
}
