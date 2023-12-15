// Copyright 2023 Vasyliev Ivan

#include <algorithm>
#include <random>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>
#include "task_3/vasyliev_i_shell_sort_n_bathcer/shell_sort_n_bathcer.h"

std::vector<int> getRandomVector(int n, int lb, int ub) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(lb, ub);
    std::vector<int> vec(n);
    for (int i = 0; i < n; ++i) vec[i] = distribution(gen);
    return vec;
}

void shellSort(std::vector<int>& array) {
    int n = array.size();
    for (int interval = n / 2; interval > 0; interval /= 2) {
        for (int i = interval; i < n; i++) {
            int j, temp = array[i];
            for (j = i; j >= interval && array[j - interval] > temp; j -= interval) {
                array[j] = array[j - interval];
            }
            array[j] = temp;
        }
    }
}

std::vector<std::pair<int, int>> comparators;

void Split(std::vector<int> procs_up, std::vector<int> procs_down) {
    int proc_count = procs_up.size() + procs_down.size();
    if (proc_count == 1) return;
    else if (proc_count == 2) {
        comparators.push_back({procs_up[0], procs_down[0]});
        return;
    }

    auto procs_up_odd = std::vector<int>(procs_up.size() / 2 + procs_up.size() % 2);
    auto procs_down_odd = std::vector<int>(procs_down.size() / 2 + procs_down.size() % 2);
    auto procs_up_even = std::vector<int>(procs_up.size() / 2);
    auto procs_down_even = std::vector<int>(procs_down.size() / 2);


    for (int i = 0; i < procs_up.size(); i++) {
        if (i % 2)  procs_up_even[i/2] = procs_up[i];
        else        procs_up_odd[i/2] = procs_up[i];
    }
    for (int i = 0; i < procs_down.size(); i++) {
        if (i % 2)  procs_down_even[i/2] = procs_down[i];
        else        procs_down_odd[i/2] = procs_down[i];
    }

    Split(procs_up_odd, procs_down_odd);
    Split(procs_up_even, procs_down_even);

    std::vector<int> procs_result(procs_up.begin(), procs_up.end());
    procs_result.insert(procs_result.end(), procs_down.begin(), procs_down.end());

    for (int i = 1; i + 1 < procs_result.size(); i += 2) {
        comparators.push_back({procs_result[i], procs_result[i + 1]});
    }
}

void UpDownDive(std::vector<int> procs) {
    if (procs.size() == 1) return;

    auto procs_up = std::vector<int>(procs.begin(), procs.begin() + procs.size() / 2);
    auto procs_down = std::vector<int>(procs.begin() + procs.size() / 2, procs.end());

    UpDownDive(procs_up);
    UpDownDive(procs_down);
    Split(procs_up, procs_down);
}

void batcherCompSetup(int proc_count) {
    auto procs = std::vector<int>(proc_count);
    for (int i = 0; i < procs.size(); i++) procs[i] = i;
    UpDownDive(procs);
}

std::vector<int> ParallelSort(std::vector<int> vec, int vec_sz) {
    boost::mpi::communicator world;
    int part_sz = vec_sz / world.size();  //! vec_sz % world.size() = 0 (for batcher simplicity)
    std::vector<int> part(part_sz);
    boost::mpi::scatter(world, vec, part.data(), part_sz, 0);

    shellSort(part);
    batcherCompSetup(world.size());

    std::vector<int> elems_internal = part;
    std::vector<int> elems_external = std::vector<int>(part_sz);
    std::vector<int> elems_temp = std::vector<int>(part_sz);

    for (int i = 0; i < comparators.size(); i++) {
        auto comparator = comparators[i] ;
        if (world.rank() == comparator.first) {
            world.send(comparator.second, i, elems_internal);
            world.recv(comparator.second, i, elems_external);

            for (int int_idx = 0, ext_idx = 0, tmp_idx = 0; tmp_idx < part_sz; tmp_idx++) {
                int result = elems_internal[int_idx];
                int current = elems_external[ext_idx];
                if (result < current) {
                    elems_temp[tmp_idx] = result;
                    int_idx++;
                } else {
                    elems_temp[tmp_idx] = current;
                    ext_idx++;
                }
            }

            std::swap(elems_internal, elems_temp);
        } else if (world.rank() == comparator.second) {
            world.recv(comparator.first, i, elems_external);
            world.send(comparator.first, i, elems_internal);

            int start = part_sz - 1;
            for (int int_idx = start, ext_idx = start, tmp_idx = start; tmp_idx >= 0; tmp_idx--) {
                int result = elems_internal[int_idx];
                int current = elems_external[ext_idx];
                if (result > current) {
                    elems_temp[tmp_idx] = result;
                    int_idx--;
                } else {
                    elems_temp[tmp_idx] = current;
                    ext_idx--;
                }
            }

            std::swap(elems_internal, elems_temp);
        }
        world.barrier();
    }
    boost::mpi::gather(world, elems_internal.data(), part_sz, vec, 0);
    return vec;
}
