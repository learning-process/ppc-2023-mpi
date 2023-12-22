  // Copyright 2023 Kozyreva Katya
#include <mpi.h>
#include <vector>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <utility>
#include "../tasks/task_3/kozyreva_k_strongin/alg_strongin.h"

coords SequentalStrongin(const std::function<double(double)>& func, double a, double b) {
    std::vector<coords> v;
    v.push_back(coords(a, func(a)));
    v.push_back(coords(b, func(b)));
    double M = 0;
    double eps = 0.0001;
    double r = 2.0;
    size_t k = 2;
    size_t t = 0;
    for (; (v[t + 1].first - v[t].first) > eps; ++k) {
        for (size_t i = 0; i < (k - 1u); ++i) {
            double M_tmp = std::abs((v[i + 1].second - v[i].second) / (v[i + 1].first - v[i].first));
            if (M_tmp > M)
                M = M_tmp;
        }
        double m = 1.0;
        if (M != 0.0) {
            m = r * M;
        }
        t = 0;
        double R = m * (v[1].first - v[0].first) + (v[1].second -
        v[0].second) *(v[1].second - v[0].second) / (m *
        (v[1].first - v[0].first)) - 2 * (v[1].second + v[0].second);
        for (size_t i = 1; i < (k - 1u); ++i) {
            double R_tmp = m * (v[i + 1u].first - v[i].first) +
            (v[i + 1u].second - v[i].second) *(v[i + 1u].second -
            v[i].second) / (m * (v[i + 1u].first - v[i].first)) -
            2 * (v[i + 1u].second + v[i].second);
            if (R_tmp > R) {
                t = i;
                R = R_tmp;
            }
        }
        double x_t1 = (v[t].first + v[t + 1].first) / 2 - (v[t + 1].second - v[t].second) / (2 * m);
        coords t1_pair = coords(x_t1, func(x_t1));
        v.insert(std::lower_bound(v.begin(), v.end(), t1_pair, [](const coords& a, const coords& b) {
            return a.first <= b.first;
        }), t1_pair);
    }
    return v[t + 1];
}

coords ParallelStrongin(const std::function<double(double)>& func, double a, double b) {
    int size, rank;
    double eps = 0.0001;
    double r = 2.0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        std::vector<coords> v;
        v.push_back(coords(a, func(a)));
        v.push_back(coords(b, func(b)));
        double M = 0.0;
        int k = 2;
        std::vector<std::pair<int, double> > tj_v;
        for (; true; k = static_cast<int>(v.size())) {
            int tj_size = (static_cast<int>(tj_v.size()) < size) ? static_cast<int>(tj_v.size()) : size;
            for (int i = 0; i < (k - 1); ++i) {
                if ((v[i + 1].first - v[i].first) < eps) {
                    int stop = 1;
                    for (int i = 1; i < size; ++i) {
                        MPI_Send(&stop, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                    }
                    coords min = v[0];
                    for (int j = 1; j < k; ++j) {
                        if (v[j].second < min.second)
                            min = v[j];
                    }
                    return min;
                }
            }
            for (int i = 0; i < (k - 1); ++i) {
                double M_tmp = std::abs((v[i + 1].second - v[i].second) / (v[i + 1].first - v[i].first));
                if (M_tmp > M)
                    M = M_tmp;
            }
            double m = 1.0;
            if (M != 0.0)
                m = r * M;
            tj_v.clear();
            double R;
            for (int i = 0; i < (k - 1); ++i) {
                R = m * (v[i + 1].first - v[i].first) + (v[i + 1].second
                - v[i].second) *(v[i + 1].second - v[i].second) /
                (m * (v[i + 1].first - v[i].first)) -2 * (v[i + 1].second +
                v[i].second); tj_v.push_back(coords(i, R));
            }
            tj_size = (static_cast<int>(tj_v.size()) < size) ? static_cast<int>(tj_v.size()) : size;
            for (int j = 0; j < tj_size; ++j) {
                for (int l = j + 1; l < static_cast<int>(tj_v.size()); ++l) {
                    if (tj_v[l].second > tj_v[j].second) {
                         std::swap(tj_v[l], tj_v[j]);
                    }
                }
            }
            double x_t0 = (v[tj_v[0].first + 1].first + v[tj_v[0].first].first)
            / 2 -(v[tj_v[0].first + 1].second - v[tj_v[0].first].second) / (2 * m);
            std::vector<coords> tmp_v(tj_size);
            tmp_v[0] = coords(x_t0, 0.0);
            for (int i = 1; i < tj_size; ++i) {
                double x_t = (v[tj_v[i].first + 1].first + v[tj_v[i].first].first)
                / 2 -(v[tj_v[i].first + 1].second - v[tj_v[i].first].second) / (2 * m);
                int stop = 0;
                MPI_Send(&stop, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&x_t, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
                tmp_v[i] = (coords(x_t, 0.0));
            }
            tmp_v[0].second = func(x_t0);
            for (int i = 1; i < tj_size; ++i) {
                MPI_Status status;
                MPI_Recv(&tmp_v[i].second, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
            }
            for (auto & t_pair : tmp_v) {
                 v.insert(std::lower_bound(v.begin(), v.end(), t_pair,
                 [](const coords& a, const coords& b) {
                      return a.first <= b.first;
                 }), t_pair);
            }
        }
    } else {
        double x;
        int stop = 0;
        MPI_Status status;
        while (stop == 0) {
            MPI_Recv(&stop, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            if (stop != 0)
                return coords();
            MPI_Recv(&x, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
            double y = func(x);
            MPI_Send(&y, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
    }
    return coords();
}
