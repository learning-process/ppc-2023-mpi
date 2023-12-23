// Copyright 2023 Kashin Stepa
#include "task_1/kashin_s_nearest_neighbor_elements/neighbor_elements.h"

int difference(const int& a, const int& b) {
    return (a > b ? a - b : b - a);
}

std::pair<int, int> neighbor_elements(const std::vector<int>& v) {
    if (v.size() < 2) return std::pair<int, int>(-1, -1);

    std::pair<int, int> ans;
    ans.first = difference(v[0], v[1]);
    ans.second = 0;

    for (int i = 1; i < v.size() - 1; i++) {
        if (difference(v[i], v[i + 1]) < ans.first) {
            ans.first = difference(v[i], v[i + 1]);
            ans.second = i;
        }
    }
    return ans;
}

std::pair<int, int> par_neighbor_elements(const std::vector<int>& v) {
    int world_size = 0, rank = 0;

    if (v.size() < 2) return std::pair<int, int>(-1, -1);

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int chunk = static_cast<int>(v.size() / world_size);
    const int tail = static_cast<int>(v.size() % world_size);

    std::vector<int> send_counts(world_size);
    std::vector<int> parts(world_size);

    for (int i = 0; i < world_size - 1; ++i) {
        send_counts[i] = chunk + 1;
        parts[i] = i * chunk;
    }

    if (tail == 0) {
        send_counts[world_size - 1] = chunk;
    } else {
        send_counts[world_size - 1] = static_cast<int>(v.size() - (world_size - 1) * chunk);
    }

    parts[world_size - 1] = (world_size - 1) * chunk;

    std::pair<int, int> answer(-1, -1);

    std::vector<int> local_arr(send_counts[rank]);

    MPI_Scatterv(
            v.data(),
            send_counts.data(),
            parts.data(), MPI_INT,
            local_arr.data(),
            send_counts[rank],
            MPI_INT,
            0,
            MPI_COMM_WORLD);

    std::pair<int, int> local_max_diff = neighbor_elements(local_arr);
    local_max_diff.second += parts[rank];

    MPI_Reduce(&local_max_diff,
               &answer,
               1,
               MPI_2INT,
               MPI_MINLOC,
               0,
               MPI_COMM_WORLD);

    return answer;
}

std::vector<int> create_random_vector(size_t size, unsigned int minDiff, unsigned int maxDiff) {
    std::vector<int> v(size);

    if (v.size() < 2) return v;

    std::random_device dev;
    std::mt19937 gen(dev());

    v[0] = gen() % 100;
    for (int i = 1; i < v.size(); i++) {
        v[i] = gen() % (maxDiff-minDiff) + minDiff + v[i-1];
    }

    return v;
}

