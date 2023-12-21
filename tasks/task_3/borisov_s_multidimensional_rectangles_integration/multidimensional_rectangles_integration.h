// Copyright 2023 Borisov Saveliy
#ifndef TASKS_TASK_2_BORISOV_S_MULTIDIMENSIONAL_RECTANGLES_INTEGRATION_MULTIDIMENSIONAL_RECTANGLES_INTEGRATION_H_
#define TASKS_TASK_2_BORISOV_S_MULTIDIMENSIONAL_RECTANGLES_INTEGRATION_MULTIDIMENSIONAL_RECTANGLES_INTEGRATION_H_

#include <cmath>
#include <vector>
#include <random>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/serialization/utility.hpp>

std::pair<double, double> getRandomLimits(int minimum_value, int maximum_value);

int getAreasNumber(const std::vector<int>& partitions_number);

double calculateIntegralRectangles(int first_area, int last_area,
                      int variables_number, double integrand(const std::vector<double>&),
                      const std::vector<int>& partitions_number,
                      const std::vector<std::pair<double, double>>& integration_limits);

double integrateSequentially(int variables_number, double integrand(const std::vector<double>&),
                      const std::vector<int>& partitions_number,
                      const std::vector<std::pair<double, double>>& integration_limits);

double integrateInParallel(int variables_number, double integrand(const std::vector<double>&),
                      const std::vector<int>& partitions_number,
                      std::vector<std::pair<double, double>> integration_limits);



#endif  // TASKS_TASK_2_BORISOV_S_MULTIDIMENSIONAL_RECTANGLES_INTEGRATION_MULTIDIMENSIONAL_RECTANGLES_INTEGRATION_H_
