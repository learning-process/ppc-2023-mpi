// Copyright 2023 Borisov Saveliy
#include "task_3/borisov_s_multidimensional_rectangles_integration/multidimensional_rectangles_integration.h"

std::pair<double, double> getRandomLimits(int minimum_value, int maximum_value) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_real_distribution<double> distrib(minimum_value, maximum_value);

    double first = distrib(gen);
    double second = distrib(gen);

    if (first > second) {
        double temp = first;
        first = second;
        second = temp;
    }

    return std::make_pair(first, second);
}

int getAreasNumber(const std::vector<int>& partitions_number) {
    int areas_number = 1;
    for (int partition : partitions_number) {
        areas_number *= partition;
    }
    return areas_number;
}

double calculateIntegralRectangles(int first_area_num, int last_area_num, int variables_number,
                                   double integrand(const std::vector<double>&),
                                   const std::vector<int>& partitions_number,
                                   const std::vector<std::pair<double, double>>& integration_limits) {
    double result = 0.0;

    for (int area_num = first_area_num; area_num < last_area_num; area_num++) {
        std::vector<double> variables(variables_number);
        int remaining_index = area_num;

        double volume = 1.0;
        for (int i = 0; i < variables_number; i++) {
            double interval = integration_limits[i].second - integration_limits[i].first;
            double step = interval / static_cast<double>(partitions_number[i]);

            double offset;
            if (i == variables_number - 1) {
                offset = static_cast<double>(remaining_index) * step + step / 2;
            } else {
                int num_steps = remaining_index / partitions_number[i];
                offset = static_cast<double>(num_steps) * step + step / 2;
                remaining_index %= partitions_number[i];
            }
            variables[i] = integration_limits[i].first + offset;

            volume *= step;
        }
        result += integrand(variables) * volume;
    }

    return result;
}

double integrateSequentially(int variables_number, double integrand(const std::vector<double>&),
                             const std::vector<int>& partitions_number,
                             const std::vector< std::pair<double, double>>& integration_limits) {
    int areas_number = getAreasNumber(partitions_number);

    double result = calculateIntegralRectangles(0, areas_number, variables_number, integrand,
                                                partitions_number, integration_limits);
    return result;
}

double integrateInParallel(int variables_number, double integrand(const std::vector<double>&),
                           const std::vector<int>& partitions_number,
                           std::vector<std::pair<double, double>> integration_limits) {
    boost::mpi::communicator comm;
    double global_result = 0;

    integration_limits.resize(variables_number);
    boost::mpi::broadcast(comm, integration_limits.data(), variables_number, 0);

    int global_areas_number = getAreasNumber(partitions_number);
    int local_areas_number = global_areas_number / comm.size();

    std::vector<double> variables(variables_number);
    int first_area = local_areas_number * comm.rank();
    int last_area = first_area + local_areas_number;
    if (comm.rank() == comm.size() - 1) {
        last_area += global_areas_number % comm.size();
    }

    double local_result = calculateIntegralRectangles(first_area, last_area, variables_number,
                                                      integrand, partitions_number, integration_limits);
    boost::mpi::reduce(comm, local_result, global_result, std::plus(), 0);
    return global_result;
}

