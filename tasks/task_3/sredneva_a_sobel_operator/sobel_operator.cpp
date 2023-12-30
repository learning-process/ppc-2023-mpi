// Copyright 2023 Sredneva Anastasiya
#include "task_3/sredneva_a_sobel_operator/sobel_operator.h"
#include <random>
#include <algorithm>

int getSobelX(int i) {
    std::vector<int> kernelX = { -1, 0, 1, -2, 0, 2, -1, 0, 1};
    return kernelX[i];
}

int getSobelY(int i) {
    std::vector<int> kernelY = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
    return kernelY[i];
}

std::vector<uint8_t> getRandomPicture(int n, int m, uint8_t l, uint8_t r) {
    int size = (n + 2) * (m + 2);
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<int> distrib(l, r);
    std::vector<uint8_t> pict(size);
    for (int i = 0; i < n + 2; i++) {
        for (int j = 0; j < m + 2; j++) {
            if (i == 0 || i == (n + 1) || j == 0 || j == (m + 1)) {
                pict[i * (m + 2) + j] = 0;
            } else {
                pict[i * (m + 2) + j] = distrib(gen);
            }
        }
    }
    return pict;
}

std::vector<uint8_t> getSequentialSobel(const std::vector<uint8_t>& pict, int n, int m) {
    int mn = m + 2;
    int nn = n + 2;
    std::vector<uint8_t> pict2;
    for (int i = 1; i < nn - 1; i++) {
        int kx, ky = 0;
        double res = 0.0;
        for (int j = 1; j < mn - 1; j++) {
            kx = getSobelX(0) * pict[(i - 1) * mn + (j - 1)] +
                getSobelX(1) * pict[(i - 1) * mn + j] +
                getSobelX(2) * pict[(i - 1) * mn + (j + 1)] +
                getSobelX(3) * pict[i * mn + (j - 1)] +
                getSobelX(4) * pict[i * mn + j] +
                getSobelX(5) * pict[i * mn + (j + 1)] +
                getSobelX(6) * pict[(i + 1) * mn + (j - 1)] +
                getSobelX(7) * pict[(i + 1) * mn + j] +
                getSobelX(8) * pict[(i + 1) * mn + (j + 1)];
            ky = getSobelY(0) * pict[(i - 1) * mn + (j - 1)] +
                getSobelY(1) * pict[(i - 1) * mn + j] +
                getSobelY(2) * pict[(i - 1) * mn + (j + 1)] +
                getSobelY(3) * pict[i * mn + (j - 1)] +
                getSobelY(4) * pict[i * mn + j] +
                getSobelY(5) * pict[i * mn + (j + 1)] +
                getSobelY(6) * pict[(i + 1) * mn + (j - 1)] +
                getSobelY(7) * pict[(i + 1) * mn + j] +
                getSobelY(8) * pict[(i + 1) * mn + (j + 1)];
            res = sqrt(kx * kx + ky * ky);
            pict2.push_back(std::min(res, 255.0));
        }
    }
    return pict2;
}

std::vector<uint8_t> getParallelSobel(const std::vector<uint8_t>& pict, int n, int m) {
    int ProcRank, ProcNum;
    int nn = n + 2;
    int mn = m + 2;
    std::vector<uint8_t> pictRes(n * m);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int size = n / ProcNum;
    int ost = n % ProcNum;
    std::vector<int> sendcounts(ProcNum);
    std::vector<int> displs(ProcNum);
    std::vector<int> recvcounts(ProcNum);
    std::vector<int> displs2(ProcNum);
    std::vector<uint8_t> pict2((size + 2) * mn);
    if (ProcRank == 0) {
        displs[0] = 0;
        displs2[0] = 0;
        sendcounts[0] = (size + ost + 2) * mn;
        recvcounts[0] = (size + ost) * m;
        for (int i = 1; i < ProcNum; i++) {
            sendcounts[i] = (size + 2) * mn;
            recvcounts[i] = (size) * m;
            displs[i] = displs[i - 1] + sendcounts[i - 1] - mn * 2;
            displs2[i] = displs2[i - 1] + recvcounts[i - 1];
        }
        pict2.resize((size + ost + 2) * mn);
    }
    MPI_Scatterv(pict.data(), sendcounts.data(), displs.data(), MPI_UINT8_T, pict2.data(),
        pict2.size(), MPI_UINT8_T, 0, MPI_COMM_WORLD);

     std::vector<uint8_t> pict3(n * m);

     int k = pict2.size() / mn;

     pict3 = getSequentialSobel(pict2, k - 2, m);

     MPI_Gatherv(pict3.data(), pict3.size(), MPI_UINT8_T, pictRes.data(), recvcounts.data(),
         displs2.data(), MPI_UINT8_T, 0, MPI_COMM_WORLD);

    return pictRes;
}
