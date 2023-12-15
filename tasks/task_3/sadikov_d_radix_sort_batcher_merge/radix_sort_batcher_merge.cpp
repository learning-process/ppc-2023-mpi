// Copyright 2023 Sadikov Damir
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <functional>
#include <utility>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

#include "task_3/sadikov_d_radix_sort_batcher_merge/radix_sort_batcher_merge.h"

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

void iterativeRadixSort(std::vector<double>& v) {
    std::vector<double> positive;
    std::vector<double> negative;
    constexpr int sz = sizeof(double);
    using ull = unsigned long long;
    ull mask_fb = 1ll << (sz * 8 - 1);
    for (auto& j : v) {
        ull temp = *reinterpret_cast<ull*>(&j);
        if (temp & mask_fb) {
            negative.push_back(j);
        } else {
            positive.push_back(j);
        }
    }

    std::vector<std::vector<double>> parts(256);
    // sort negative
    for (int t = 0; t < sz; t++) {
        for (auto& j : negative) {
            ull temp = *reinterpret_cast<ull*>(&j);
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
    // sort positive
    for (int t = 0; t < sz; t++) {
        for (auto& j : positive) {
            ull temp = *reinterpret_cast<ull*>(&j);
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
}

void compAndSwap(double& a, double& b) {
    if (a > b) std::swap(a, b);
}

void parallelBatcherMergeOfRadixSort(std::vector<double>& v, int n) {
    boost::mpi::communicator world;
    int world_size = world.size();
    int world_rank = world.rank();

    // use only 2 processes
    if (world_size < 2) {
        iterativeRadixSort(v);
        return;
    }
    if (world_rank >= 2) {
        return;
    }

    // v.size() == 2 ^ k, k >= 2
    if (n < 4 || (n & (n - 1)) != 0) {
        iterativeRadixSort(v);
        return;
    }

    std::vector<double> t;
    t.resize(n / 2);

    // Сортировка половинок
    if (world_rank == 0) {
        world.send(1, 0, v.data() + n / 2, n / 2);
        std::copy(v.begin(), v.begin() + n / 2, t.begin());
        iterativeRadixSort(t);
        std::copy(t.begin(), t.end(), v.begin());
        world.recv(1, 0, v.data() + n / 2, n / 2);
    } else {
        world.recv(0, 0, t.data(), n / 2);
        iterativeRadixSort(t);
        world.send(0, 0, t.data(), n / 2);
    }

    // Batcher Merge

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
}
