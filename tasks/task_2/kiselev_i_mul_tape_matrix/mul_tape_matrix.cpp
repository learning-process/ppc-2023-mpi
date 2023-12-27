// Copyright 2023 Kiselev Igor
#include "task_2/kiselev_i_mul_tape_matrix/mul_tape_matrix.h"

std::vector<int> SeqMulMatrix(const std::vector<int> &a, const std::vector<int> &b, int n, int m) {
    std::vector<int> resMatrix(n * n);

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < m; k++) {
                resMatrix[j * n + i] += a[j * m + k] * b[k * n + i];
            }
        }
    }
    return resMatrix;
}
std::vector<int> ParMulMatrix(std::vector<int> * SMM, std::vector<int> * PMM, int n, int m) {
    std::vector<int> &a = *SMM;
    std::vector<int> &b = *PMM;

    int sizeProc = 0;
    int rankProc = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

    int chain_s = n / sizeProc;
    int reminder = n % sizeProc;

    MPI_Bcast(b.data(), m * n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rankProc == 0) {
        if (chain_s > 0) {
            for (int proc = 1; proc < sizeProc; proc++) {
                MPI_Send(a.data() + reminder * m + proc * chain_s * m,
                m * chain_s, MPI_INT, proc, 0, MPI_COMM_WORLD);
            }
        }
    }

    std::vector<int> locVec;

    if (rankProc == 0) {
        locVec = std::vector<int>(a.begin(), a.begin() + chain_s * m + reminder * m);
    } else {
        if (chain_s > 0) {
            locVec.resize(m * chain_s);
            MPI_Status status;
            MPI_Recv(locVec.data(), m * chain_s, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        }
    }

    std::vector<int> locRes((locVec.size() / m) * (locVec.size() / m));
    locRes = SeqMulMatrix(locVec, b, locVec.size() / m, m);
    std::vector<int> resMatrix(n * n);
    std::vector<int> send_counts(sizeProc);
    std::vector<int> displs(sizeProc);

    for (int i = 0; i < sizeProc; i++) {
        send_counts[i] = (i == 0) ? chain_s * m + reminder * m : chain_s * m;
        displs[i] = (i == 0) ? 0 : (displs[i - 1] + send_counts[i - 1]);
    }

    MPI_Gatherv(locRes.data(), send_counts[rankProc], MPI_INT, resMatrix.data(),
    send_counts.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);
    if (rankProc == 0) {
        return resMatrix;
    } else {
        return std::vector<int> {};
    }
}
// kiselev_i
