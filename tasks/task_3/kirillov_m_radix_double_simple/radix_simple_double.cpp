// Copyright 2023 Kirillov Maxim
#include <vector>
#include <random>
#include <algorithm>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_3/kirillov_m_radix_double_simple/radix_simple_double.h"

std::vector<double> getRandomVector(int size, double min, double max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<double> vector(size);
    for (int  i = 0; i < size; i++) {
        double r = static_cast<double>(gen()) / std::mt19937::max();
        vector[i] = min + r * (max - min);
    }
    return vector;
}

void countSort(double* in, int digit, int size) {
    std::vector<double> res(size);
    int count[10] = {0};

    for (int i = 0; i < size; i++) {
        count[static_cast<int>(in[i] * 1e6 / digit) % 10]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = size - 1; i >= 0; i--) {
        res[count[static_cast<int>(in[i] * 1e6 / digit) % 10]-- - 1] = in[i];
    }

    std::copy(res.begin(), res.end(), in);
}

std::vector<double> getSeqRadixSortSimpleMerge(const std::vector<double>& vec) {
    std::vector<double>res(vec);
    double max = *std::max_element(vec.begin(), vec.end());
    for (int i = 1; max / i > 0; i *= 10) {
        countSort(res.data(), i, res.size());
    }
    return res;
}

std::vector<double> simpleMerge(const std::vector<double>& first,
                                const std::vector<double>& second) {
    std::vector<double> res;
    res.reserve(first.size() + second.size());
    int i = 0, j = 0;

    while (i < first.size() && j < second.size()) {
        res.push_back((first[i] < second[j]) ? first[i++] : second[j++]);
    }

    while (i < first.size()) {
        res.push_back(first[i++]);
    }
    while (j < second.size()) {
        res.push_back(second[j++]);
    }

    return res;
}

std::vector<double> getParRadixSortSimpleMerge(const std::vector<double>& vec) {
    boost::mpi::communicator world;
    std::vector<double> res(vec.size(), 0);
    int dataCount;
    if (world.rank() == 0) {
        dataCount = vec.size();
    }
    boost::mpi::broadcast(world, dataCount, 0);
    int dataPerProcess = dataCount / world.size(),
        reminder = dataCount % world.size();

    std::vector<int> sizes(world.size(), dataPerProcess);
    for (int i = 0; i < reminder; i++) {
        sizes[i]++;
    }
    std::vector<double> local(sizes[world.rank()], 0);
    if (world.rank() == 0)
        boost::mpi::scatterv(world, vec, sizes,
                             local.data(), 0);
    else
        boost::mpi::scatterv(world, local.data(),
                             sizes[world.rank()], 0);
    local = getSeqRadixSortSimpleMerge(local);
    if (world.rank() == 0) {
        boost::mpi::gatherv(world, local,
                            res.data(), sizes, 0);
    } else {
        boost::mpi::gatherv(world, local, 0);
    }

    if (world.rank() == 0) {
        std::vector<double> temp;
        for (int i = 1; i < world.size(); i++) {
            int leftCurr = 0, rightCurr = (i < reminder) ?
                    (dataPerProcess + 1) * i : (dataPerProcess + 1) *
                    reminder + (i - reminder) * dataPerProcess;
            int leftNext = rightCurr, rightNext = (i < reminder) ?
                    rightCurr + dataPerProcess + 1 : rightCurr + dataPerProcess;
            temp = simpleMerge(
                  std::vector<double>(res.begin() + leftCurr,
                  res.begin() + leftNext), std::vector<double>(
                  res.begin() + rightCurr, res.begin() + rightNext));

            std::copy(temp.begin(), temp.end(),
                      res.begin() + leftCurr);
        }
    }

    return res;
}
