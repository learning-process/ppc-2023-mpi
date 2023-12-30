// Copyright 2023 Gontsov Roman
#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "task_3/gontsov_r_monte_carlo_integrate/monte_carlo_integrate.h"

const double relative_error = 0.1;

TEST(Monte_Carlo_std, Test_one_dimension) {
    const std::function<double(const std::vector<double>)> f =
        [](const std::vector<double>& x) -> double { return x[0] * x[0]; };
    std::vector<double> a = { 0.0 }, b = { 10.0 };
    int number_points = 100000;

    double calc_value_integral = getIntegralMonteCarlo(f, a, b, number_points);
    double calc_value_integral_std =
    getIntegralMonteCarloStd(f, a, b, number_points);
    double error = std::abs(calc_value_integral - calc_value_integral_std)
    / std::abs(calc_value_integral);

    ASSERT_LE(error, relative_error);
}

TEST(Monte_Carlo_std, Test_two_dimension) {
    const std::function<double(const std::vector<double>)> f =
        [](const std::vector<double>& x) ->
        double { return x[0] * x[0] + x[1] * x[1];
        };
    std::vector<double> a = { 0.0, -1.0 }, b = { 5.0, 1.0 };
    int number_points = 100000;

    double calc_value_integral = getIntegralMonteCarlo(f, a, b, number_points);
    double calc_value_integral_std =
     getIntegralMonteCarloStd(f, a, b, number_points);
    double error = std::abs(calc_value_integral - calc_value_integral_std)
     / std::abs(calc_value_integral);

    ASSERT_LE(error, relative_error);
}

TEST(Monte_Carlo_std, Test_three_dimension) {
    const std::function<double(const std::vector<double>)> f =
        [](const std::vector<double>& x) ->
         double { return x[0] * x[1] * x[1] * x[2] + x[1] * x[2] / 2.0;
          };
    std::vector<double> a = { 0.0, -1.0, 5.0 }, b = { 5.0, 1.0, 10.0 };
    int number_points = 100000;

    double calc_value_integral = getIntegralMonteCarlo(f, a, b, number_points);
    double calc_value_integral_std =
     getIntegralMonteCarloStd(f, a, b, number_points);
    double error = std::abs(calc_value_integral - calc_value_integral_std)
     / std::abs(calc_value_integral);

    ASSERT_LE(error, relative_error);
}

TEST(Monte_Carlo_std, Test_four_dimension) {
    const std::function<double(const std::vector<double>)> f =
            [](const std::vector<double>& x) -> double {
                return std::exp(x[0]) * x[1] * x[1] * std::pow(x[3], 5) -
                       std::exp(x[1]) * x[0] / x[2] + x[1] / std::pow(x[0], 3);
            };
    std::vector<double> a = { 1.0, -1.0, 5.0, -10.0 },
     b = { 5.0, 1.0, 10.0, -9.0 };
    int number_points = 100000;

    double calc_value_integral = getIntegralMonteCarlo(f, a, b, number_points);
    double calc_value_integral_std =
     getIntegralMonteCarloStd(f, a, b, number_points);
    double error = std::abs(calc_value_integral - calc_value_integral_std)
     / std::abs(calc_value_integral);

    ASSERT_LE(error, relative_error);
}

TEST(Monte_Carlo_std, Test_five_dimension) {
    const std::function<double(const std::vector<double>)> f =
            [](const std::vector<double>& x) -> double {
                return std::exp(x[0]) * x[1] * x[1] * std::pow(x[3], 5)
                 - std::exp(x[1]) * x[0] * x[4] * x[4] / x[2] +
                       x[1] / std::pow(x[0], 3);
            };
    std::vector<double> a = { 1.0, -1.0, 5.0, -10.0, -3.0 },
     b = { 5.0, 1.0, 10.0, -9.0, 5.0 };
    int number_points = 100000;

    double calc_value_integral = getIntegralMonteCarlo(f, a, b, number_points);
    double calc_value_integral_std =
     getIntegralMonteCarloStd(f, a, b, number_points);
    double error = std::abs(calc_value_integral - calc_value_integral_std)
     / std::abs(calc_value_integral);

    ASSERT_LE(error, relative_error);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
