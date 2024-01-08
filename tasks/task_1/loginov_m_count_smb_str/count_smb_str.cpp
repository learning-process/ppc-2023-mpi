// Copyright 2023 Loginov Maxim
#include "task_1/loginov_m_count_smb_str/count_smb_str.h"

int calcFreqSeq(const std::string& str, char target) {
    int frequency = 0;
    for (char c : str) {
        if (c == target) {
            frequency++;
        }
    }
    return frequency;
}

int calcFreqPar(const std::string& str, char target) {
    int numProc;
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    int rankProc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

    int strSize = str.length();

    std::vector<int> recv_counts(numProc);
    std::vector<int> displs(numProc);

    int chunk_size = strSize / numProc;
    int remainder = strSize % numProc;

    for (int i = 0; i < numProc; i++) {
        recv_counts[i] = chunk_size;
        if (i < remainder) {
            recv_counts[i]++;
        }
        displs[i] = (i > 0) ? (displs[i - 1] + recv_counts[i - 1]) : 0;
    }

    std::string recv_data(recv_counts[rankProc], '\0');
    MPI_Scatterv(str.data(), recv_counts.data(), displs.data(), MPI_CHAR,
        recv_data.data(), recv_counts[rankProc], MPI_CHAR, 0, MPI_COMM_WORLD);

    int localFreq = calcFreqSeq(recv_data, target);
    int globalFreq = 0;

    MPI_Reduce(&localFreq, &globalFreq, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return globalFreq;
}
