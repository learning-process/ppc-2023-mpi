// Copyright 2023 Lesnikov Nikita

#include "task_3/lesnikov_glob_opt_char_par/glob_opt_char_par.h"

#include <iostream>
#include <string>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>


double findM(const std::vector<double>& X, const std::function<double(double)>& f) {
    double M = 0;
    for (int i = 0; i < X.size() - 1; i++) {
        double abs_ = std::abs(f(X[i]) - f(X[i + 1]));
        double abs_x = std::abs(X[i + 1] - X[i]);
        M = std::max(M, abs_ / abs_x);
    }
    return M;
}

double getm(double M, double r) {
    if (M) {
        return r * M;
    }
    return 1;
}

std::vector<double> getR(double m, const std::vector<double>& X, std::function<double(double)> f) {
    std::vector<double> R(X.size() - 1);
    for (int i = 0; i < X.size() - 1; i++) {
        double abs_x = std::abs(X[i + 1] - X[i]);
        double sq = (f(X[i]) - f(X[i + 1])) * (f(X[i]) - f(X[i + 1]));
        double fsum = (f(X[i]) + f(X[i + 1]));
        R[i] = m * abs_x + sq / m / abs_x - 2 * fsum;
    }
    return R;
}

double getXk_1(double xt_1, double xt, double r, double m, std::function<double(double)> f) {
    int sign;
    if (f(xt) - f(xt_1) > 0) {
        sign = 1;
    } else {
        sign = -1;
    }

    double mean = ((xt_1 + xt) / 2);

    double abs_ = std::abs(f(xt) - f(xt_1));

    return mean - sign * abs_ / m / 2;
}

int getMax(const std::vector<double>& v) {
    int maxIndex = 0;
    for (int i = 0; i < v.size(); i++) {
        if (v[maxIndex] < v[i]) {
            maxIndex = i;
        }
    }
    return maxIndex;
}

double getMinSequential(std::function<double(double)> f, double leftBound,
    double rightBound, double eps, int maxIterations, double r) {

    std::vector<double> X = { leftBound, rightBound };

    double lastX = 0;

    for (int i = 0; i < maxIterations; i++) {
        double M = findM(X, f);
        double m = getm(M, r);
        std::vector<double> R = getR(m, X, f);
        int maxRindex = getMax(R);

        double xk_1 = getXk_1(X[maxRindex], X[maxRindex + 1], r, m, f);

        if (xk_1 < leftBound) {
            xk_1 = leftBound;
        } else if (xk_1 > rightBound) {
            xk_1 = rightBound;
        }

        lastX = xk_1;

        double abs_ = std::abs(xk_1 - X[maxRindex]);

        if (abs_ < eps) {
            return xk_1;
        }

        auto xk_1Place = std::upper_bound(X.begin(), X.end(), xk_1);

        X.insert(xk_1Place, xk_1);
    }

    return lastX;
}

double getMinParallel(std::function<double(double)> f, double leftBound,
    double rightBound, double eps, int maxIterations, double r) {

    boost::mpi::communicator world;

    std::vector<double> X;

    if (world.rank() == 0) {
        X = { leftBound, rightBound };
    }

    double lastX = 0;

    bool end = false;

    for (int i = 0; i < maxIterations; i++) {
        std::vector<double> loc_X;

        int usefulWorldSize = 0;

        if (world.rank() == 0) {
            usefulWorldSize = std::min(static_cast<int>(world.size()), static_cast<int>(X.size()));
        }

        if (world.rank() == 0 && end) {
            usefulWorldSize = -1;
        }

        if (world.size() > 1) {
            boost::mpi::broadcast(world, usefulWorldSize, 0);
        }

        if (usefulWorldSize == -1) {
            break;
        }

        int part_size = 0;
        int remainder = 0;

        if (usefulWorldSize <= world.rank()) {
            continue;
        }

        if (world.rank() == 0) {
            remainder = X.size() % usefulWorldSize;
            part_size = X.size() / usefulWorldSize;

            int not_end = static_cast<int>(usefulWorldSize != 1);
            loc_X = std::vector<double>(X.begin(), X.begin() + part_size + remainder + not_end);

            for (int j = 1; j < usefulWorldSize; j++) {
                not_end = static_cast<int>(j != usefulWorldSize - 1);
                std::vector<double> temp(X.begin() + remainder + part_size * j
                    , X.begin() + remainder + part_size * (j + 1) + not_end);
                size_t temp_size = temp.size();
                int dd = 0;
                world.send(j, 0, temp);
                world.recv(j, 0, dd);

                if (temp_size < 2) {
                    usefulWorldSize--;
                }
            }
        } else {
            world.recv(0, 0, loc_X);
            int dd = 0;
            world.send(0, 0, dd);
        }

        if (world.rank() != 0 && loc_X.size() < 2) {
            usefulWorldSize--;
        }

        if (loc_X.size() < 2) {
            continue;
        }

        double loc_M = findM(loc_X, f);
        double M = loc_M;
        double m;

        if (world.rank() == 0) {
            double temp_M = 0;
            for (int j = 1; j < usefulWorldSize; j++) {
                world.recv(j, 0, temp_M);
                M = std::max(M, temp_M);
            }
        } else {
            world.send(0, 0, loc_M);
        }

        if (world.rank() == 0) {
            for (int j = 1; j < usefulWorldSize; j++) {
                world.send(j, 0, M);
            }
        } else {
            world.recv(0, 0, M);
        }

        m = getm(M, r);

        std::vector<double> R = getR(m, loc_X, f);
        int maxRindex = getMax(R);

        double maxRValue = 0;

        if (maxRindex < R.size() && maxRindex >= 0) {
            maxRValue = R[maxRindex];
        } else {
            maxRValue = 0;
        }

        if (world.rank() == 0) {
            for (int j = 1; j < usefulWorldSize; j++) {
                int temp_index = 0;
                world.recv(j, 0, temp_index);
                int realIndex = temp_index + remainder + j * part_size;
                double locR = 0;
                world.recv(j, 0, locR);

                if (maxRValue < locR) {
                    maxRValue = locR;
                    maxRindex = realIndex;
                }
            }

            double xk_1 = getXk_1(X[maxRindex], X[maxRindex + 1], r, m, f);

            if (xk_1 < leftBound) {
                xk_1 = leftBound + 0.01;
            } else if (xk_1 > rightBound) {
                xk_1 = rightBound - 0.01;
            }

            lastX = xk_1;

            if (std::abs(xk_1 - X[maxRindex]) < eps) {
                end = true;
                continue;
            }

            auto xk_1Place = std::upper_bound(X.begin(), X.end(), xk_1);

            X.insert(xk_1Place, xk_1);
        } else {
            world.send(0, 0, maxRindex);
            world.send(0, 0, maxRValue);
        }
    }

    world.barrier();

    return lastX;
}
