// Copyright 2023 Gontsov Roman
#include <utility>
#include <random>
#include <iostream>
#include "task_3/gontsov_r_monte_carlo_integrate/monte_carlo_integrate.h"
#include "../../../3rdparty/unapproved/unapproved.h"


double getIntegralMonteCarlo(
    const std::function<double(const std::vector<double>&)>& f,
     std::vector<double> a, std::vector<double> b, int number_points) {
    int dimension = static_cast<int>(a.size());
    std::mt19937 gen;
    std::vector<std::uniform_real_distribution<double>>
     uniform_distribution(dimension);
    std::vector<double> point(dimension);
    double result = 0.0;

    for (int i = 0; i < dimension; i++) {
        uniform_distribution[i] =
         std::uniform_real_distribution<double>(a[i], b[i]);
    }

    for (int i = 0; i < number_points; i++) {
        for (int j = 0; j < dimension; j++) {
            point[j] = uniform_distribution[j](gen);
        }
        result += f(point);
    }
    for (int i = 0; i < dimension; i++) {
        result *= (b[i] - a[i]);
    }
    result /= number_points;

    return result;
}

double getIntegralMonteCarloStd(
    const std::function<double(const std::vector<double>&)>& f,
    std::vector<double> a, std::vector<double> b, int number_points) {
    int dimension = static_cast<int>(a.size());
    std::vector<std::uniform_real_distribution<double>>
     uniform_distribution(dimension);
    double result = 0.0;

    for (int i = 0; i < dimension; i++) {
        uniform_distribution[i] =
         std::uniform_real_distribution<double>(a[i], b[i]);
    }

    const int numberThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(numberThreads);
    std::vector<std::promise<double>> promises(numberThreads);
    std::vector<std::future<double>> futures(numberThreads);

    for (int i = 0; i < numberThreads; i++) {
        futures[i] = promises[i].get_future();
        threads[i] = std::thread([&, pr = std::move(promises[i])] () mutable {
            double res = 0.0;
            int size = number_points / numberThreads
             + ((i < number_points % numberThreads) ? 1 : 0);
            std::vector<double> point(dimension);
            std::mt19937 gen;

            for (int j = 0; j < size; j++) {
                for (int j = 0; j < dimension; j++) {
                    point[j] = uniform_distribution[j](gen);
                }
                res += f(point);
            }
            pr.set_value(res);
        });
    }

    for (int i = 0; i < numberThreads; i++) {
        threads[i].join();
        result += futures[i].get();
    }

    for (int i = 0; i < dimension; i++) {
        result *= (b[i] - a[i]);
    }
    result /= number_points;

    return result;
}
