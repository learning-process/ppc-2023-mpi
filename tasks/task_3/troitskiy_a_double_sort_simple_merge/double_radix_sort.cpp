// Copyright 2023 Troitskiy Alexandr

#include "tasks/task_3/troitskiy_a_double_sort_simple_merge/double_radix_sort.h"

void SortByCount(double *input, double *temp, int byte_num, int n) {
    std::vector<std::vector<int>> count(256);
    unsigned char *mas = (unsigned char *) input;
    for (int i = 0; i < n; i++) {
        count[mas[8 * i + byte_num]].push_back(i);
    }
    int cur = 0;
    for (int i = 0; i < 256; i++) {
        for (auto j : count[i]) {
            temp[cur++] = input[j];
        }
    }
}

std::vector<double> DoubleRadixSortSeq(std::vector<double> vec) {
    int n = vec.size();

    int negative_count = std::count_if(vec.begin(), vec.end(), [](int num){
        return num < 0;
    });

    double *positives = new double[n - negative_count];
    double *negatives = new double[negative_count];

    double *temp_positive = new double[n - negative_count];
    double *temp_negative = new double[negative_count];

    int cur = 0;
    for (int i = 0; i < n; i++) {
        if (vec[i] > 0) {
            positives[cur] = vec[i];
            cur++;
        } else {
            negatives[i - cur] = - vec[i];
        }
    }

    for (int i = 0; i < 8; i += 2) {
        SortByCount(positives, temp_positive, i, n - negative_count);
        SortByCount(temp_positive, positives, i + 1, n - negative_count);
    }
    for (int i = 0; i < 8; i += 2) {
        SortByCount(negatives, temp_negative, i, negative_count);
        SortByCount(temp_negative, negatives, i + 1, negative_count);
    }

    for (int i = negative_count - 1; i >= 0; i--) {
        vec[negative_count - 1 - i] = -negatives[i];
    }
    for (int i = 0; i < n - negative_count; i++) {
        vec[i + negative_count] = positives[i];
    }
    delete[]temp_positive;
    delete[]temp_negative;
    delete[]positives;
    delete[]negatives;
    return vec;
}

std::vector<double> MergeVectors(const std::vector<double> &vec1, const std::vector<double> &vec2) {
    std::vector<double> merged(vec1.size() + vec2.size());
    int count1 = 0;
    int count2 = 0;
    while (count1 < vec1.size() || count2 < vec2.size()) {
        if (count1 == vec1.size()) {
            merged[count1 + count2] = vec2[count2];
            count2++;
        } else if (count2 == vec2.size()) {
            merged[count1 + count2] = vec1[count1];
            count1++;
        } else {
            if (vec1[count1] < vec2[count2]) {
                merged[count1 + count2] = vec1[count1];
                count1++;
            } else {
                merged[count1 + count2] = vec2[count2];
                count2++;
            }
        }
    }
    return merged;
}

std::vector<double> DoubleRadixSortParallel(std::vector<double> vec) {
    int n = vec.size();
    int rank = 0;
    int procs_count = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs_count);
    int size, offset;
    int delta = n / procs_count;
    int remain = n % procs_count;
    size = delta;
    std::vector<int> disps;
    std::vector<double> batch(delta);
    std::vector<int> sent_counts;
    if (rank == 0) {
        sent_counts.resize(procs_count, delta);
        sent_counts[0] += remain;
        disps.resize(procs_count, 0);
        for (int i = 1; i < procs_count; i++) {
            disps[i] = disps[i - 1] + sent_counts[i - 1];
        }
        batch.resize(sent_counts[0]);
    }
    MPI_Scatterv(vec.data(), sent_counts.data(), disps.data(), MPI_DOUBLE, batch.data(), batch.size(),
                 MPI_DOUBLE, 0, MPI_COMM_WORLD);
    batch = DoubleRadixSortSeq(batch);
    if (delta == 0) {
        return batch;
    }
    int count = 1;
    std::vector<double> vec_to;
    while (count < procs_count) {
        if (rank % (2 * count) == 0) {
            if (rank + count < procs_count) {
                // first proc
                int sz;
                if (rank + count + count >= procs_count) {
                    sz = procs_count - (rank + count);
                } else {
                    sz = count;
                }
                vec_to.resize(sz * delta);
                MPI_Status status;
                MPI_Recv(vec_to.data(), sz * delta, MPI_DOUBLE, rank + count, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                batch = MergeVectors(batch, vec_to);
            }
        } else if (rank % count == 0) {
            // second proc
            vec_to = batch;
            MPI_Send(vec_to.data(), vec_to.size(), MPI_DOUBLE, rank - count, 0, MPI_COMM_WORLD);
        }
        count *= 2;
        MPI_Barrier(MPI_COMM_WORLD);
    }
    return batch;
}

std::vector<double> generateRandomDoubleVector(int n, double min, double max) {
    std::random_device device;
    std::mt19937 gen_engine(device());
    std::vector<double> result(n);
    for (int i = 0; i < n; i++) {
        int x = static_cast<int>(gen_engine());
        result[i] = min + x * (max - min) / RAND_MAX;
        if (result[i] < 0) {
            result[i] *= -1;
        }
    }
    return result;
}
