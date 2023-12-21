// Copyright 2023 Kashirin Alexander


#include "task_1/kashirin_a_num_of_sent/num_of_sent.h"


int numSeq(const std::vector<char>& str) {
    int num = 0;
    for (char c : str) {
        if (c == '/' || c == '!' || c == '?') num++;
    }
    return num;
}

void generateRandomString(const std::string& str, int size) {

    std::string alphabet = "abcdefghijklmnopqrstuvwxyz.?!";
	for (int i = 0; i < size; i++) {
        str  += alphabet[rand() % 30];
    }
}

int numPar(const std::string& str) {
    int numProc;
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int strSize = str.size();
    std::vector<int> displs(numProc);
    std::vector<int> sendcount(numProc);
    std::vector<int> recvcount(numProc);

    int chunk_size = strSize / numProc;
    int remain = strSize % numProc;

    for (int i = 0; i < numProc; i++) {
        if (i == 0) {
            sendcount[i] = recvcount[i] = chunk_size + remain;
            displs[i] = 0;
        } else {
            sendcount[i] = recvcount[i] = chunk_size;
            displs[i] = sendcount[i - 1] + displs[i - 1];
        }
    }

    std::vector<char>  recvdata(recvcount[rank]);
    MPI_Scatterv(str.c_str(), sendcount.data(), displs.data(), MPI_CHAR,
        recvdata.data(), recvcount[rank], MPI_CHAR, 0, MPI_COMM_WORLD);

    int localNum = numSeq(recvdata);
    int globalNum = 0;
    MPI_Reduce(&localNum, &globalNum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return globalNum;
}
