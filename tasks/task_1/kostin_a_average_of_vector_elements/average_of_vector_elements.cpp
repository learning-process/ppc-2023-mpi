// Copyright 2023 Kostin Artem
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <functional>
#include "task_1/kostin_a_average_of_vector_elements/average_of_vector_elements.h"

std::vector<int> getRandomVector(int sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(sz);
    for (int  i = 0; i < sz; i++) { vec[i] = gen() % 100; }
    return vec;
}

int getSequentialSum(std::vector<int> vec) {
    const int  sz = vec.size();
    int reduction_elem = 0;
    for (int  i = 0; i < sz; i++) {
        reduction_elem += vec[i];
    }
    return reduction_elem;
}

double getParallelAverage(std::vector<int> vec) {
    int size_vector = vec.size();
    if (size_vector == 0)
        return 0;
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int part_size = size_vector / world_size;
    int remainder_size = size_vector % world_size;
    std::vector<int> send_counts(world_size, part_size);
    std::vector<int> displs(world_size);
    for (int i = 0; i < world_size; i++) {
        if (i < remainder_size)
            send_counts[i]++;
        if (i > 0)
            displs[i] = displs[i - 1] + send_counts[i - 1];
    }
    std::vector<int> recv_buf(send_counts[world_rank]);
    MPI_Scatterv(vec.data(), send_counts.data(), displs.data(), MPI_INT,
                 recv_buf.data(), send_counts[world_rank], MPI_INT, 0, MPI_COMM_WORLD);
    int generalSum = 0;
    int partialSum = getSequentialSum(recv_buf);
    MPI_Reduce(&partialSum, &generalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    double DgeneralSum = static_cast<double> (generalSum);
    double Dsize_vector = static_cast<double> (size_vector);
    return DgeneralSum / Dsize_vector;
}
