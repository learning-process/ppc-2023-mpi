// Copyright 2023 Troitskiy Alexandr

#include "task_3/troitskiy_a_double_sort_simple_merge/double_radix_sort.h"

void CountingSort(double *inp, double *tmp, int byteNum, int n) {
    unsigned char *mas = (unsigned char *) inp;
    std::vector <std::vector<int>> count(256);
    for (int i = 0; i < n; i++) {
        count[mas[8 * i + byteNum]].push_back(i);
    }
    int cur = 0;
    for (int i = 0; i < 256; i++) {
        for ( auto j : count[i] ) {
            tmp[cur] = inp[j];
            cur++;
        }
    }
}

std::vector<double> SequenceRadixSortDouble(std::vector<double> a, int n) {
    int cnt_neg = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] < 0)
            cnt_neg++;
    }

    double *inp_pos = new double[n - cnt_neg];
    double *tmp_pos = new double[n - cnt_neg];

    double *inp_neg = new double[cnt_neg];
    double *tmp_neg = new double[cnt_neg];
    int cur = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] > 0) {
            inp_pos[cur] = a[i];
            cur++;
        } else {
            inp_neg[i - cur] = -a[i];
        }
    }
    CountingSort(inp_pos, tmp_pos, 0, n - cnt_neg);
    CountingSort(tmp_pos, inp_pos, 1, n - cnt_neg);
    CountingSort(inp_pos, tmp_pos, 2, n - cnt_neg);
    CountingSort(tmp_pos, inp_pos, 3, n - cnt_neg);
    CountingSort(inp_pos, tmp_pos, 4, n - cnt_neg);
    CountingSort(tmp_pos, inp_pos, 5, n - cnt_neg);
    CountingSort(inp_pos, tmp_pos, 6, n - cnt_neg);
    CountingSort(tmp_pos, inp_pos, 7, n - cnt_neg);


    CountingSort(inp_neg, tmp_neg, 0, cnt_neg);
    CountingSort(tmp_neg, inp_neg, 1, cnt_neg);
    CountingSort(inp_neg, tmp_neg, 2, cnt_neg);
    CountingSort(tmp_neg, inp_neg, 3, cnt_neg);
    CountingSort(inp_neg, tmp_neg, 4, cnt_neg);
    CountingSort(tmp_neg, inp_neg, 5, cnt_neg);
    CountingSort(inp_neg, tmp_neg, 6, cnt_neg);
    CountingSort(tmp_neg, inp_neg, 7, cnt_neg);

    for (int i = cnt_neg - 1; i >= 0; i--) {
        a[cnt_neg - 1 - i] = -inp_neg[i];
    }
    for (int i = 0; i < n - cnt_neg; i++) {
        a[i + cnt_neg] = inp_pos[i];
    }
    delete[]tmp_pos;
    delete[]tmp_neg;
    delete[]inp_pos;
    delete[]inp_neg;
    return a;
}

std::vector<double> merge(const std::vector<double> &a, const std::vector<double> &b) {
    int sz = a.size() + b.size();
    std::vector<double> ans(sz);
    int l = 0;
    int r = 0;
    for (int i = 0; i < a.size() + b.size(); i++) {
        if (l == a.size()) {
            ans[i] = b[r];
            r++;
        } else if (r == b.size()) {
            ans[i] = a[l];
            l++;
        } else {
            if (a[l] < b[r]) {
                ans[i] = a[l];
                l++;
            } else {
                ans[i] = b[r];
                r++;
            }
        }
    }
    return ans;
}

std::vector<double> ParallelRadixSortDouble(std::vector<double> a, int n) {
    int rank = 0;
    int countProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    int size, offset;
    int delta = n / countProc;
    int remain = n % countProc;
    size = delta;
    std::vector<int> sendcounts;
    std::vector<int> displs;
    std::vector<double> piece(delta);
    if (rank == 0) {
        sendcounts.resize(countProc, delta);
        sendcounts[0] += remain;
        displs.resize(countProc, 0);
        for (int i = 1; i < countProc; i++) {
            displs[i] = displs[i - 1] + sendcounts[i - 1];
        }
        piece.resize(sendcounts[0]);
    }
    MPI_Scatterv(a.data(), sendcounts.data(), displs.data(), MPI_DOUBLE, piece.data(), piece.size(),
                 MPI_DOUBLE, 0, MPI_COMM_WORLD);
    piece = SequenceRadixSortDouble(piece, piece.size());
    if (delta == 0) {
        return piece;
    }
    int cnt = 1;
    std::vector<double> to;
    while (cnt < countProc) {
        if (rank % (2 * cnt) == 0) {
            if (rank + cnt < countProc) {
                // left proc
                int sz;
                if (rank + cnt + cnt >= countProc) {
                    sz = countProc - (rank + cnt);
                } else {
                    sz = cnt;
                }
                to.resize(sz * delta);
                MPI_Status status;
                MPI_Recv(to.data(), sz * delta, MPI_DOUBLE, rank + cnt, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                piece = merge(piece, to);
            }
        } else if (rank % cnt == 0) {
            // right proc
            to = piece;
            MPI_Send(to.data(), to.size(), MPI_DOUBLE, rank - cnt, 0, MPI_COMM_WORLD);
        }
        cnt *= 2;
        MPI_Barrier(MPI_COMM_WORLD);
    }
    return piece;
}

std::vector<double> getRandomInput(int n, double fMin, double fMax) {
    std::random_device device;
    std::mt19937 generator(device());
    std::vector<double> inp(n);
    for (int i = 0; i < n; i++) {
        int f = static_cast<int>(generator());
        inp[i] = fMin + f * (fMax - fMin) / RAND_MAX;
        if (inp[i] < 0) inp[i] *= -1;
    }
    return inp;
}
