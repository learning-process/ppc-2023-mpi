// Copyright 2023 Kanakov Roman
#ifndef TASKS_TASK_3_KANAKOV_MONTE_CARLO_MONTE_CARLO_H_
#define TASKS_TASK_3_KANAKOV_MONTE_CARLO_MONTE_CARLO_H_

#include <cstdint>
#include <array>
#include <functional>

template <size_t dim>
double multiple_integral_monte_carlo_parallel(
        const std::function<double(std::array<double, dim>)> &f,
        const std::function<bool(std::array<double, dim>)> &in_region,
        const std::array<double[2], dim> &rect,
        int64_t n);

template <size_t dim>
double multiple_integral_monte_carlo_sequential(
        const std::function<double(std::array<double, dim>)> &f,
        const std::function<bool(std::array<double, dim>)> &in_region,
        const std::array<double[2], dim> &rect,
        int64_t n);

#endif  // TASKS_TASK_3_KANAKOV_MONTE_CARLO_MONTE_CARLO_H_
