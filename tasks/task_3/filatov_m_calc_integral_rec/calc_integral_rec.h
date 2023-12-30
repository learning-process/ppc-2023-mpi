// Copyright 2023 Filatov Maxim

#ifndef TASKS_TASK_3_FILATOV_M_CALC_INTEGRAL_REC_CALC_INTEGRAL_REC_H_
#define TASKS_TASK_3_FILATOV_M_CALC_INTEGRAL_REC_CALC_INTEGRAL_REC_H_

#include <cmath>
#include <vector>
#include <random>
#include <utility>
#include <numeric>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/mpi.hpp>

#define DEFAULT_RANDOM_MAX 10000
#define DEFAULT_RANDOM_MIN -10000

std::pair<double, double> getRandomLimits(int minimum_value, int maximum_value);

int getAreasNumber(const std::vector<int>& partitions_number);

double calculateIntegralRectangles(
    int first_area,
    int last_area,
    int variables_number,
    double integrand(const std::vector<double>&),
    const std::vector<int>& partitions_number,
    const std::vector<std::pair<double, double>>& integration_limits);

double integrateSequentially(
    int variables_number,
    double integrand(const std::vector<double>&),
    const std::vector<int>& partitions_number,
    const std::vector<std::pair<double, double>>& integration_limits);

double integrateInParallel(
    int variables_number,
    double integrand(const std::vector<double>&),
    const std::vector<int>& partitions_number,
    std::vector<std::pair<double, double>> integration_limits);



#endif  // TASKS_TASK_3_FILATOV_M_CALC_INTEGRAL_REC_CALC_INTEGRAL_REC_H_
