// Copyright 2023 Kosarev Egor
#include <mpi.h>
#include <climits>
#include <iostream>
#include <algorithm>
#include <random>
#include "task_1/kosarev_e_min_values_across_the_columns_matrix/min_values.h"

std::vector<int> getRandomMatrix(int n, int m) {
    std::random_device device;
    std::mt19937 generator(device());
    constexpr int mod = 1000;
    std::vector<int> ans(n * m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            ans[i * m + j] = generator() % mod - mod / 2;
        }
    }
    return ans;
}

std::vector<int> getParallelMin(const std::vector<int>& A, int n, int m) {
    int rank = 0;
    int countProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);

    int delta = m / countProc;
    int remain = m % countProc;
    std::vector<int> answer;
    std::vector<int> sendcounts;
    std::vector<int> displs;
    std::vector<int> mydata(delta * n);

    if (rank == 0) {
        answer.resize(m, INT_MAX);
        sendcounts.resize(countProc, delta);
        sendcounts[0] += remain;
        displs.resize(countProc, 0);
        for (int i = 1; i < countProc; i++) {
            displs[i] = displs[i - 1] + sendcounts[i - 1];
        }
        mydata.resize((delta + remain) * n);
    }

    MPI_Datatype mytype, column;
    MPI_Type_vector(n, 1, m, MPI_INT, &mytype);
    MPI_Type_commit(&mytype);
    MPI_Type_create_resized(mytype, 0, sizeof(int) * 1, &column);
    MPI_Type_commit(&column);
    MPI_Scatterv(A.data(), sendcounts.data(), displs.data(), column, mydata.data(),
        static_cast<int>(mydata.size()), MPI_INT, 0, MPI_COMM_WORLD);
    std::vector<int> anst;
    int sizemx = mydata.size() / n;
    for (int i = 0; i < sizemx; i++) {
        int mn = INT_MAX;
        for (int j = 0; j < n; j++) {
            mn = std::min(mn, mydata[i * n + j]);
        }
        anst.push_back(mn);
    }

    MPI_Gatherv(anst.data(), anst.size(), MPI_INT, answer.data(), sendcounts.data(), displs.data(), MPI_INT, 0,
        MPI_COMM_WORLD);
    return answer;
}

std::vector<int> getSequentialMin(const std::vector<int>& A, int n, int m) {
    std::vector<int> ans(m);
    for (int i = 0; i < m; i++) {
        int mn = 1e9;
        for (int j = 0; j < n; j++) {
            mn = std::min(mn, A[j * m + i]);
        }
        ans[i] = mn;
    }
    return ans;
}