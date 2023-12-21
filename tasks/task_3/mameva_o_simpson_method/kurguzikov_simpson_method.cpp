// Copyright 2023 Mamaeva Olga
#include <mpi.h>
#include <random>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include "../tasks/task_3/mameva_o_simpson_method/kurguzikov_simpson_method.h"

double ParallelVersion(double (*func)(std::vector<double>),
        std::vector <std::pair<double, double>> v, std::vector <int> distr) {
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int n = v.size();
    std::vector<int> ni(n);
    std::vector<double> hi(n);
    std::vector<std::pair<double, double >> ranges(n);
    int num_elem;
    if (procRank == 0) {
        num_elem = 1;
        for (int i = 0; i < n; ++i) {
            ni[i] = distr[i];
            hi[i] = (v[i].second - v[i].first) / ni[i];
            num_elem *= ni[i];
            ranges[i] = v[i];
        }
    }
    MPI_Bcast(&num_elem, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(ni.data(), n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(hi.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(ranges.data(), 2 * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    int ost = num_elem % procNum;
    int count = num_elem / procNum + (procRank < ost ? 1 : 0);
    int suf = 0;
    if (procRank < ost) {
        suf = procRank * count;
    } else {
        suf = ost * (count + 1) + (procRank - ost) * count;
    }
    double sum = 0.0;
    for (int i = 0; i < count; ++i) {
        std::vector<std::vector<double>> params(n);
        int number = suf + i;
        for (int j = 0; j < n; ++j) {
            int cef = number % ni[j];
            params[j].push_back(ranges[j].first + cef * hi[j]);
            params[j].push_back(ranges[j].first + cef * hi[j] + hi[j] / 2);
            params[j].push_back(ranges[j].first + cef * hi[j] + hi[j] / 2);
            params[j].push_back(ranges[j].first + cef * hi[j] + hi[j] / 2);
            params[j].push_back(ranges[j].first + cef * hi[j] + hi[j] / 2);
            params[j].push_back(ranges[j].first + cef * hi[j] + hi[j]);
            number /= ni[j];
        }
        std::vector<double> arg;
        for (int i = 0; i < pow(6, n); ++i) {
            int tmp = i;
            for (int j = 0; j < n; ++j) {
                arg.push_back(params[j][tmp % 6]);
                tmp /= 6;
            }
            sum += func(arg);
            arg.clear();
        }
        params.clear();
    }
    double gsum = 0.0;
    MPI_Reduce(&sum, &gsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    for (int i = 0; i < n; ++i) {
        gsum *= hi[i]/6.0;
    }
    return gsum;
}

double SequentialVersion(double (*func)(std::vector<double>),
        std::vector <std::pair<double, double>> v, std::vector <int> distr) {
    int n = distr.size();
    std::vector<double> h(n);
    int64_t number = 1;
    for (int i = 0; i < n; ++i) {
        h[i] = (v[i].second - v[i].first) / distr[i];
        number *= distr[i];
    }
    double sum = 0.0;
    for (int i = 0; i < number; ++i) {
        std::vector<std::vector<double>> params(n);
        int x = i;
        for (int j = 0; j < n; ++j) {
            int cef = x % distr[j];
            params[j].push_back(v[j].first + cef * h[j]);
            params[j].push_back(v[j].first + cef * h[j] + h[j] / 2);
            params[j].push_back(v[j].first + cef * h[j] + h[j] / 2);
            params[j].push_back(v[j].first + cef * h[j] + h[j] / 2);
            params[j].push_back(v[j].first + cef * h[j] + h[j] / 2);
            params[j].push_back(v[j].first + cef * h[j] + h[j]);
            x /= distr[j];
        }
        std::vector<double> arg;
        for (int i = 0; i < pow(6, n); ++i) {
            int tmp = i;
            for (int j = 0; j < n; ++j) {
                arg.push_back(params[j][tmp % 6]);
                tmp /= 6;
            }
            sum += func(arg);
            arg.clear();
        }
        params.clear();
    }
    for (int i = 0; i < n; ++i) {
        sum *= h[i]/6.0;
    }
    return sum;
}
