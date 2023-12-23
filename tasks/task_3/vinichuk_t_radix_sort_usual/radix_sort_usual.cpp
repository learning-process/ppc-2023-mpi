// Copyright 2023 Vinichuk Timofey
#include <algorithm>
#include <random>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_3/vinichuk_t_radix_sort_usual/radix_sort_usual.h"

std::vector<int> getRandomVector(int sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(sz);
    for (int i = 0; i < sz; i++) { vec[i] = gen() % 10000; }
    return vec;
}


std::vector<int> SeqRadixSortUsualMerge(const std::vector<int>& arr) {
    int max = *std::max_element(begin(arr), end(arr));
    std::vector<int> count(10, 0), out(arr.size(), 0), ar(arr);
    for (int exp = 1; max / exp > 0; exp *= 10) {
        for (int i = 0; i < ar.size(); i++) {
            count[(ar[i] / exp) % 10]++;
        }
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }
        for (int i = ar.size() - 1; i >= 0; i--) {
            out[--count[(ar[i] / exp) % 10]] = ar[i];
        }
        count.assign(10, 0);
        ar = out;
        out.assign(out.size(), 0);
    }
    return ar;
}

std::vector<int> ParallelRadixSortUsualMerge(const std::vector<int>& arr) {
    boost::mpi::communicator world;
    std::vector<int> output(arr.size(), 0);
    int n;
    if (world.rank() == 0) {
        n = arr.size();
    }
    boost::mpi::broadcast(world, n, 0);
    int p = n / world.size(), b = n % world.size();
    std::vector<int> sizes;
    for (int i = 0; i < world.size(); i++) {
        sizes.push_back((i < b) ? p + 1 : p);
    }
    std::vector<int> local(sizes[world.rank()], 0);
    if (world.rank() == 0)
        boost::mpi::scatterv(world, arr, sizes, local.data(), 0);
    else
        boost::mpi::scatterv(world, local.data(), sizes[world.rank()], 0);
    int max = *std::max_element(begin(local), end(local));
    std::vector<int> count(10, 0), out(local.size(), 0);
    for (int exp = 1; max / exp > 0; exp *= 10) {
        for (int i = 0; i < local.size(); i++) {
            count[(local[i]/exp)%10]++;
        }
        for (int i = 1; i < 10; i++) {
            count[i]+= count[i-1];
        }
        for (int i = local.size()-1; i >=0; i--) {
            out[--count[(local[i]/exp)%10]] = local[i];
        }
        count.assign(10, 0);
        local = out;
        out.assign(out.size(), 0);
    }
    if (world.rank() == 0) {
        boost::mpi::gatherv(world, local, output.data(), sizes, 0);
    } else {
        boost::mpi::gatherv(world, local, 0);
    }
    std::vector<int> temp;
    if (world.rank() == 0) {
        int l, r, ll, rr;
        for (int i = 1; i < world.size(); i++) {
            l = 0;
            r = (i < b) ? (p + 1) * i : (p + 1) * b + (i - b) * p;
            ll = r;
            rr = (i < b) ? r + p + 1 : r + p;
            while (l < ll && r < rr) {
                if (output[l] < output[r]) {
                    temp.push_back(output[l]);
                    l++;
                } else {
                    temp.push_back(output[r]);
                    r++;
                }
            }
            while (l < ll) {
                temp.push_back(output[l]);
                l++;
            }
            while (r < rr) {
                temp.push_back(output[r]);
                r++;
            }
            for (int j = 0; j < rr; j++) {
                output[j] = temp[j];
            }
            temp.clear();
        }
    }

    return output;
}
