// Copyright 2023 Kiselev Igor
#include "task_2/kiselev_i_mul_tape_matrix/mul_tape_matrix.h"

std::vector<int> SeqMulMatrix(const std::vector<int>& a, const std::vector<int>& b, int an, int am, int bm) {
    std::vector<int> resMatrix(an * bm, 0);

    for (int i = 0; i < an; i++) {
        for (int j = 0; j < bm; j++) {
            for (int k = 0; k < am; k++) {
                resMatrix[i * bm + j] +=
                    a[i * am + k]
                    * b[k * bm + j];
            }
        }
    }
    return resMatrix;
}

std::vector<int> ParMulMatrix(std::vector<int> *a, std::vector<int> *b, int an, int am) {
    std::vector<int>& mat_a = *a;
    std::vector<int>& mat_b = *b;
    int sizeProc, rankProc;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

    if (sizeProc > an || sizeProc == 1) {
        return rankProc == 0 ?
            SeqMulMatrix(mat_a, mat_b, an, am, an) :
            std::vector<int>{};
    }

    int bn = am;
    int bm = an;
    int chain_s = an / sizeProc;
    int reminder = an % sizeProc;

    if (rankProc == 0) {
        for (int proc = 1; proc < sizeProc; proc++) {
            int chainA = proc * chain_s * am;
            int chainB = proc * chain_s;

            if (reminder != 0) {
                chainA += reminder * am;
                chainB += reminder;
            }

            std::vector<int> sendB(chain_s * bn, 0);
            for (int i = 0; i < sendB.size(); i++) {
                int chain_n = i % chain_s;
                int chain_m = (i / chain_s) * bm;

                sendB[i] = mat_b[chainB + chain_n + chain_m];
            }

            MPI_Send(mat_a.data() + chainA, chain_s * am, MPI_INT, proc, 1, MPI_COMM_WORLD);
            MPI_Send(sendB.data(), sendB.size(), MPI_INT, proc, 2, MPI_COMM_WORLD);
        }
    }
    int lSize = (chain_s + reminder) * am;

    std::vector<int> locA(lSize, 0);
    std::vector<int> locB(lSize + 1, 0);

    if (rankProc == 0) {
        for (int i = 0; i < lSize; i++) {
            int chain_n = i % (chain_s + reminder);
            int chain_m = (i / (chain_s + reminder)) * bm;
            locB[i] = mat_b[chain_n + chain_m];
            locA[i] = mat_a[i];
        }
    } else {
        MPI_Status status;
        MPI_Recv(locA.data(), static_cast<int>(lSize - reminder * am),
            MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(locB.data(), static_cast<int>(lSize - reminder * am),
            MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
    }
    locB[lSize] = rankProc == 0 ? 0 : rankProc * chain_s + reminder;

    std::vector<int> locRes(an * bm, 0);

    int snd = (rankProc + 1) % sizeProc;
    int recv = (rankProc - 1) < 0 ? sizeProc - 1 : rankProc - 1;

    for (int i = 0; i < sizeProc; i++) {
        int loc_an = rankProc == 0 ? chain_s + reminder : chain_s;
        int loc_bm = (rankProc + i) % sizeProc == 0 ? chain_s + reminder : chain_s;

        std::vector<int> tRes = SeqMulMatrix(locA, locB, loc_an, am, loc_bm);

        int chainRes = rankProc == 0 ? 0 : (rankProc * chain_s + reminder) * an;
        for (int j = 0; j < tRes.size(); j++) {
            int chain_n = locB[lSize] + j % loc_bm;
            int chain_m = (j / loc_bm) * an;

            locRes[chainRes + chain_n + chain_m] += tRes[j];
        }

        MPI_Send(locB.data(), static_cast<int>(locB.size()),
            MPI_INT, recv, i, MPI_COMM_WORLD);
        MPI_Status status;
        MPI_Recv(locB.data(), static_cast<int>(locB.size()),
            MPI_INT, snd, i, MPI_COMM_WORLD, &status);
    }

    std::vector<int> resMatrix(an * bm, 0);
    MPI_Reduce(locRes.data(), resMatrix.data(), static_cast<int>(locRes.size()),
        MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return resMatrix;
}
// kiselev_i
