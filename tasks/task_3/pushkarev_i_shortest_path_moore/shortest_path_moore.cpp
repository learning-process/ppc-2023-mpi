// Copyright 2023 Pushkarev Ilya

#include <algorithm>
#include <functional>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/operations.hpp>

#include "task_3/pushkarev_i_shortest_path_moore/shortest_path_moore.h"

std::vector<int> mooore(int rank, int procs, int n, const std::vector<int>& matr, bool* has_negative_cycle) {
    boost::mpi::communicator world;
    int loc_n;

    if (rank == 0) {
        loc_n = n;
    }
    boost::mpi::broadcast(world, loc_n, 0);

    int delta = loc_n / procs;
    int loc_start = delta * rank;
    int loc_end = delta * (rank + 1);
    if (rank == procs - 1) {
        loc_end = loc_n;
    }

    std::vector<int> loc_matr(loc_n * loc_n);
    std::vector<int> loc_dist(loc_n);
    std::vector<int> queue_active(loc_n);
    std::vector<int> queue_new(loc_n);
    std::vector<int> q_counter(loc_n);

    if (rank == 0) {
        loc_matr = matr;
    }

    boost::mpi::broadcast(world, loc_matr.data(), loc_n * loc_n, 0);

    int weight;
    bool loc_has_change;
    bool has_change;
    int iter = 0;

    for (int i = 0; i < loc_n; i++) {
        loc_dist[i] = INF;
    }
    loc_dist[iter] = 0;
    queue_active[iter] = true;
    while (true) {
        loc_has_change = false;
        std::fill(std::begin(queue_new), std::end(queue_new), false);
        for (int u = 0; u < loc_n; u++) {
            if (queue_active[u]) {  // if u is active
                for (int v = loc_start; v < loc_end; v++) {
                    weight = loc_matr[u * loc_n + v];
                    if (weight < INF) {
                        if (loc_dist[u] + weight < loc_dist[v]) {
                            loc_dist[v] = loc_dist[u] + weight;
                            queue_new[v] = 1;
                            loc_has_change = true;
                        }
                    }
                }
            }
        }

        boost::mpi::all_reduce(world, queue_new.data(), loc_n, queue_active.data(), std::logical_or<int>());
        for (int u = 0; u < loc_n; u++) {
            if (queue_active[u]) {
                q_counter[u]++;
                if (q_counter[u] >= loc_n) {
                    *has_negative_cycle = true;
                    return {};
                }
            }
        }

        boost::mpi::all_reduce(world, loc_has_change, has_change, std::logical_or<bool>());
        if (!has_change) {
            // std::cout << iter << std::endl;
            break;
        }

        boost::mpi::all_reduce(world, boost::mpi::inplace(loc_dist.data()), loc_n, boost::mpi::minimum<int>());
        iter++;
    }

    return loc_dist;
}
