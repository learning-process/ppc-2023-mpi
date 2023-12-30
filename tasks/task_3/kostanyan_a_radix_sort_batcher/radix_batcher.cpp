// Copyright 2023 Kostanyan Arsen

#include "task_3/kostanyan_a_radix_sort_batcher/radix_batcher.h"

std::vector<double> generateRandomVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<double> randomVector(size);

    for (int i = 0; i < size; i++) {
        double randomNumber = static_cast<double>(gen() % 1000) / (static_cast<double>(gen() % 1000) + 1);
        if (static_cast<double>(gen() % 4) == 2) {
            randomNumber *= -1;
        }
        randomVector[i] = randomNumber;
    }

    return randomVector;
}
std::vector<double> parallel_radix_batcher_sort(std::vector<double> to_sort_vec) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int vsize = to_sort_vec.size() / size;
    std::vector<double> lvec(vsize);


    MPI_Scatter(to_sort_vec.data(), vsize, MPI_DOUBLE,
        lvec.data(), vsize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    radix_sort(lvec.data(), lvec.size());

    if (rank != 0) {
        MPI_Send(lvec.data(), vsize, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        std::vector<std::vector<double>> vbuff;
        vbuff.push_back(lvec);
        for (int i = 1; i < size; i++) {
            MPI_Recv(lvec.data(), vsize, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            vbuff.push_back(lvec);
        }
        return seq_batcher_merges(vbuff);
    }
    return lvec;
}

static union {
    uint64_t bits;
    double d;
} value;

void sorter(double * arr, int size, int iter, int base, int * negatives_cnt) {
    double * local_arr = new double[size];
    int * cnt = new int[base]();
    int mask = base - 1;
    (*negatives_cnt) = 0;
    int ind;

    for (int i = 0 ; i < size; i++) {
        value.d = arr[i];
        ind = (((value.bits) >> (8 * iter)) & mask);
        cnt[ind]++;
    }

    for (int i = 1; i < base; i++) {
        if (i >= (base >> 1) ) (*negatives_cnt)+=cnt[i];
        cnt[i] += cnt[i - 1];
    }

    for (int i = size - 1; i >= 0; i--) {
        value.d = arr[i];
        ind = (((value.bits) >> (8 * iter)) & mask);
        local_arr[cnt[ind] - 1] = arr[i];
        cnt[ind]--;
    }

    for (int i = 0; i < size; i++)
        arr[i] = local_arr[i];

    delete[] local_arr;
    delete[] cnt;
}

void radix_sort(double * arr, int size) {
    int bits = 8;
    int base = (1 << bits);
    int iters = (sizeof(double) * 8) / bits;
    int negatives = 0;

    for (int i = 0; i < iters; i++) {
        sorter(arr, size, i, base, &(negatives));
    }

    if (negatives == 0) return;

    std::reverse(arr + size - negatives,
                arr + size);
    double * negatives_buff = new double[negatives];
    memcpy(negatives_buff, (arr + size - negatives), negatives * sizeof(double));
    memmove((arr + negatives), arr,
        (size - negatives) * sizeof(double));
    memcpy(arr, negatives_buff, negatives * sizeof(double));
}

