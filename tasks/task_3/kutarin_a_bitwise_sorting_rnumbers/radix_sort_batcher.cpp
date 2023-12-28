// Copyright 2023 Kutarin Aleksandr
#include "task_3/kutarin_a_bitwise_sorting_rnumbers/radix_sort_batcher.h"
#include <vector>
#include <random>
#include <algorithm>
#include <cstdint>
#include <utility>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

void compAndSwap(double& a, double& b) {
    if (a > b) std::swap(a, b);
}

std::vector<double> iterativeRadixSort(std::vector<double> v) {
    std::vector<double> positive;
    std::vector<double> negative;
    constexpr int sz = sizeof(double);
    using ull = uint64_t;
    ull mask_fb = 1ull << (sz * 8 - 1);
    for (auto& j : v) {
        ull temp = *reinterpret_cast<ull*>(reinterpret_cast<void*>(&j));
        if (temp & mask_fb) {
            negative.push_back(j);
        } else {
            positive.push_back(j);
        }
    }

    std::vector<std::vector<double>> parts(256);
    for (int t = 0; t < sz; t++) {
        for (auto& j : negative) {
            ull temp = *reinterpret_cast<ull*>(reinterpret_cast<void*>(&j));
            temp >>= t * 8;
            temp &= 255;
            parts[temp].push_back(j);
        }
        int i = 0;
        for (int k = 255; k >= 0; k--) {
            for (auto& j : parts[k]) {
                negative[i++] = j;
            }
            parts[k].clear();
        }
    }
    for (int t = 0; t < sz; t++) {
        for (auto& j : positive) {
            ull temp = *reinterpret_cast<ull*>(reinterpret_cast<void*>(&j));
            temp >>= t * 8;
            temp &= 255;
            parts[temp].push_back(j);
        }
        int i = 0;
        for (int k = 0; k < 256; k++) {
            for (auto& j : parts[k]) {
                positive[i++] = j;
            }
            parts[k].clear();
        }
    }
    int i = 0;
    for (auto& j : negative) {
        v[i++] = j;
    }
    for (auto& j : positive) {
        v[i++] = j;
    }

    return v;
}

std::vector<double> getRandomVector(int n) {
    std::random_device dev;
    std::mt19937_64 gen(dev());
    std::uniform_real_distribution<double> dist(-100, 100);
    std::vector<double> vec(n);
    for (auto& i : vec) {
        i = dist(gen);
    }
    return vec;
}

std::vector<double> parallelBatcherRadixSort(std::vector<double> v, int n) {
    boost::mpi::communicator world;
    int world_size = world.size();
    int world_rank = world.rank();

    if (world_size < 2) {
        v = iterativeRadixSort(v);
        return v;
    }
    if (world_rank >= 2) {
        return v;
    }

    if (n < 4 || (n & (n - 1)) != 0) {
        v = iterativeRadixSort(v);
        return v;
    }

    std::vector<double> t;
    t.resize(n / 2);

    if (world_rank == 0) {
        world.send(1, 0, v.data() + n / 2, n / 2);
        std::copy(v.begin(), v.begin() + n / 2, t.begin());
        t = iterativeRadixSort(t);
        std::copy(t.begin(), t.end(), v.begin());
        world.recv(1, 0, v.data() + n / 2, n / 2);
    } else {
        world.recv(0, 0, t.data(), n / 2);
        t = iterativeRadixSort(t);
        world.send(0, 0, t.data(), n / 2);
    }

    if (world_rank == 0) {
        world.send(1, 0, v.data() + n / 4, n / 2);
        for (int i = 0; i < n / 4; i++) {
            compAndSwap(v[i], v[n - 1 - i]);
        }
        world.recv(1, 0, v.data() + n / 4, n / 2);
    } else {
        world.recv(0, 0, t.data(), n / 2);
        for (int i = 0; i < n / 4; i++) {
            compAndSwap(t[i], t[n / 2 - 1 - i]);
        }
        world.send(0, 0, t.data(), n / 2);
    }

    std::vector<double>* T_ptr;
    if (world_rank == 0) {
        world.send(1, 0, v.data() + n / 2, n / 2);
        T_ptr = &v;
    } else {
        world.recv(0, 0, t.data(), n / 2);
        T_ptr = &t;
    }
    std::vector<double>& T = *T_ptr;

    for (int k = n / 2; k >= 2; k /= 2) {
        for (int l = 0; l < n / 2 / k; l++) {
            for (int m = 0; m < k / 2; m++) {
                compAndSwap(T[k * l + m], T[k * l + m + k / 2]);
            }
        }
    }

    if (world_rank == 0) {
        world.recv(1, 0, v.data() + n / 2, n / 2);
    } else {
        world.send(0, 0, t.data(), n / 2);
    }
    return v;
}
