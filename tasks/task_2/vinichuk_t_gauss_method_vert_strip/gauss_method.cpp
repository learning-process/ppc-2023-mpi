// Copyright 2023 Vinichuk Timofey
#include <random>
#include <algorithm>
#include <functional>
#include <vector>
#include <cstdlib>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/serialization/vector.hpp>
#include "task_2/vinichuk_t_gauss_method_vert_strip/gauss_method.h"

std::vector<double> SeqMethodGaussian(const std::vector<double>& matrix,
 int h) {
    double coef;
    std::vector<double> triangle(matrix);
    for (int i = 0; i < h - 1; i++) {
        if (triangle[(h + 1) * i + i] == 0) {
            int j = i + 1;
            while (j < h && triangle[(h + 1) * j + i] == 0) {
                j++;
            }
            for (int k = 0; k < h + 1; i++) {
                double f = triangle[(h + 1) * i + k];
                triangle[(h + 1) * i + k] = triangle[(h + 1) * j + k];
                triangle[(h + 1) * j + k] = f;
            }
        }

        for (int j = i + 1; j < h; j++) {
            coef = triangle[(h + 1) * i + i] /
                triangle[(h + 1) * j + i];
            for (int k = i + 1; k < h + 1; k++) {
                triangle[(h + 1) * j + k] *= coef;
                triangle[(h + 1) * j + k] -= triangle[(h + 1) * i + k];
            }

            triangle[(h + 1) * j + i] = 0;
        }
    }
    std::vector<double> unknown(h);
    for (int i = h - 1; i >= 0; i--) {
        double sum = 0;
        for (int j = i + 1; j < h + 1; j++) {
            triangle[i * (h + 1) + j] /= triangle[i * (h + 1) + i];
            if (j != h)
                sum += triangle[i * (h + 1) + j] * unknown[j];
        }
        triangle[i * (h + 1) + i] = 1;
        unknown[i] = triangle[i * (h + 1) + h] - sum;
    }
    return unknown;
}

std::vector<double> ParallelMethodGaussian(const std::vector<double>& matrix,
    int n) {
    boost::mpi::communicator world;
    int totalNumber;
    int p = 0;
    int h;
    if (world.rank() == 0)
        h = n;

    boost::mpi::broadcast(world, h, 0);
    std::vector<double> part;
    if (world.rank() == 0) {
        for (int i = world.size() - 1; i >= 0; i--) {
            for (int j = i; j < matrix.size(); j += h + 1) {
                for (int k = j; k < j + h + 1 - i; k += world.size()) {
                    part.push_back(matrix[k]);
                }
            }
            if (i != 0) {
                world.send(i, 0, part);
                part.clear();
            }
        }
    } else {
        world.recv(0, 0, part);
    }
    totalNumber = part.size() / h;
    world.barrier();

    //// reducing the matrix to the form of a triangle.
    std::vector<double> coef;
    for (int i = 0; i < h - 1; i++) {
        // checking whether the diagonal element is zero.
        int j = 0;
        if (world.rank() == i % world.size()) {
            if (part[totalNumber * i + i / world.size()] == 0) {
                j = i + 1;
                while (j < h && part[totalNumber * j + i / world.size()] == 0) {
                    j++;
                }
            } else {
                   j = -1;
            }
        }
        boost::mpi::broadcast(world, j, i % world.size());
        if (j != -1) {
            for (int k = 0; k < totalNumber; k++) {
                double f = part[totalNumber * i + k];
                part[totalNumber * i + k] = part[totalNumber * j + k];
                part[totalNumber * j + k] = f;
            }
        }
        world.barrier();
        // transformation.
        if (world.rank() == i % world.size()) {
            for (int j = i + 1; j < h; j++) {
                if (part[totalNumber * j + i / world.size()] != 0) {
                    coef.push_back(part[totalNumber * i + i / world.size()] /
                        part[totalNumber * j + i / world.size()]);
                    part[totalNumber * j + i / world.size()] = 0;
                } else {
                    coef.push_back(0);
                }
            }
        }

        boost::mpi::broadcast(world, coef, i % world.size());
        for (int j = i + 1; j < h; j++) {
            for (int k = 0; k < totalNumber; k++) {
                if (k * world.size() + world.rank() > i) {
                    if (coef[j - i - 1] != 0) {
                        part[totalNumber * j + k] *= coef[j-i-1];
                    part[totalNumber * j + k] -= part[totalNumber * i + k];
                    }
                }
            }
        }
        coef.clear();
        world.barrier();
    }
    // collecting parts and searching for the unknown.
    std::vector<double> unknown(h);
    if (world.rank() == 0) {
        std::vector<double> triangle(h*(h+1));
        for (int i = 0; i < world.size(); i++) {
            if (i != 0)
                world.recv(i, i, part);
            int b = 0;
            for (int j = i; j < matrix.size(); j += h + 1) {
                for (int k = j; k < j + h + 1-i; k += world.size()) {
                    triangle[k] = part[b++];
                }
            }
            b = 0;
        }
        for (int i = h-1; i >=0; i--) {
            double sum = 0;
            for (int j = i + 1; j < h + 1; j++) {
                triangle[i * (h + 1) + j] /= triangle[i * (h + 1) + i];
                if (j != h)
                    sum += triangle[i * (h + 1) + j] * unknown[j];
            }
            triangle[i * (h + 1) + i] = 1;
            unknown[i] = triangle[i * (h + 1) + h]-sum;
        }
    } else {
        world.send(0, world.rank(), part);
    }
    return unknown;
}
