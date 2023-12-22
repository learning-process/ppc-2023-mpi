// Copyright 2023 Khodyrev Fedor
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <functional>
#include "task_3/khodyrev_f_simpson_method/simpson_method.h"

double function_one_dimension(const std::vector<double>& x) {
    return x[0] * x[0];
}

double function_two_dimension(const std::vector<double>& x) {
    return x[0] * x[0] + x[1] * x[1];
}

double function_three_dimension(const std::vector<double>& x) {
    return x[0] * x[0] + x[1] * x[1] + x[2] * x[2];
}

double seq_simpson_method(double (*f)(const std::vector<double>&),
    const std::vector<double>& a,
    const std::vector<double>& b, int n) {
    int dimensions = a.size();
    std::vector<double> h(dimensions);
    std::vector<int> div_count(dimensions, 0);

    for (int i = 0; i < dimensions; ++i) {
        h[i] = (b[i] - a[i]) / n;
    }

    double result = 0.0;
    std::vector<double> x(dimensions, 0.0);
    while (true) {
        double coeff = 1.0;
        for (int i = 0; i < dimensions; ++i) {
            x[i] = a[i] + div_count[i] * h[i];
            if (div_count[i] == 0 || div_count[i] == n) {
                coeff *= 1.0;
            } else if (div_count[i] % 2 == 1) {
                coeff *= 4.0;
            } else {
                coeff *= 2.0;
            }
        }
        result += coeff * f(x);

        int dim = 0;
        while (dim < dimensions) {
            div_count[dim]++;
            if (div_count[dim] <= n) {
                break;
            }
            div_count[dim] = 0;
            dim++;
        }

        if (dim >= dimensions) {
            break;
        }
    }

    for (int i = 0; i < dimensions; ++i) {
        result *= h[i] / 3.0;
    }

    return result;
}

double par_simpson_method(double (*f)(const std::vector<double>&),
    const std::vector<double>& a,
        const std::vector<double>& b, int n) {
    int rank = 0;
    int size = 0;
    int j = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int elem_per_process = std::pow(n+1, a.size()) / size;
    int start_position = elem_per_process * rank;

    int dimensions = a.size();
    std::vector<double> h(dimensions);
    std::vector<int> div_count(dimensions, 0);

    for (int i = 0; i < a.size(); i++) {
        if (i == 0) {
            div_count[i] = start_position % (n + 1);
        } else {
            div_count[i] = static_cast<int>(start_position /
                std::pow(n + 1, i)) % (n + 1);
        }
    }

    for (int i = 0; i < dimensions; ++i) {
        h[i] = (b[i] - a[i]) / n;
    }

    double local_result = 0.0;
    double result = 0;
    std::vector<double> x(dimensions, 0.0);
    while (true) {
        double coeff = 1.0;
        for (int i = 0; i < dimensions; ++i) {
            x[i] = a[i] + div_count[i] * h[i];
            if (div_count[i] == 0 || div_count[i] == n) {
                coeff *= 1.0;
            } else if (div_count[i] % 2 == 1) {
                coeff *= 4.0;
            } else {
                coeff *= 2.0;
            }
        }
        local_result += coeff * f(x);
        j++;

        if (rank != size - 1 && j == elem_per_process) {
            break;
        }

        int dim = 0;
        while (dim < dimensions) {
            div_count[dim]++;
            if (div_count[dim] <= n) {
                break;
            }
            div_count[dim] = 0;
            dim++;
        }
        if (dim >= dimensions) {
            break;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce(&local_result, &result, 1, MPI_DOUBLE,
        MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        for (int i = 0; i < dimensions; ++i) {
            result *= h[i] / 3.0;
        }
        return result;
    }
    return 0.0;
}
