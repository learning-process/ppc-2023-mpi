// Copyright 2023 Nogin Denis

#include "task_1/nogin_d_most_different_elements/algo.h"

void maxDifFunc(void *in, void *out, int *len, MPI_Datatype *datatype) {
    std::pair<int, int> *inData = (std::pair<int, int> *)in;
    std::pair<int, int> *outData = (std::pair<int, int> *)out;

    for (int i = 0; i < *len; i++) {
        int difIn = abs(inData[i].first - inData[i].second);
        int difOut = abs(outData[i].first - outData[i].second);
        if (difIn > difOut) {
            outData[i] = inData[i];
        }
    }
}

std::pair<int, int> sequentialMostDifferentNeighborElements(const std::vector<int> &vec) {
    if (vec.size() < 2)
        return std::make_pair(0, 0);

    int maxDiff = 0;
    std::pair<int, int> maxDiffPair;
    size_t i = 1;
    for (i = 1; i < vec.size(); ++i) {
        int diff = std::abs(vec[i] - vec[i - 1]);
        if (diff > maxDiff) {
            maxDiff = diff;
            maxDiffPair = {vec[i - 1], vec[i]};
        }
    }

    return maxDiffPair;
}

std::pair<int, int> parallelMostDifferentNeighborElements(const std::vector<int> &vec) {
    int worldSize;
    int worldRank;
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    MPI_Op maxDiffOp;
    MPI_Op_create(&maxDifFunc, 1, &maxDiffOp);

    if (vec.size() < 2)
        return std::make_pair(0, 0);

    int n = vec.size();
    std::vector<int> sendCounts(worldSize);
    std::vector<int> displs(worldSize);

    for (int i = 0; i < worldSize; ++i) {
        sendCounts[i] = n / worldSize + 1;
        displs[i] = i * (sendCounts[i] - 1);
    }
    sendCounts[worldSize - 1] += n % worldSize - 1;

    std::vector<int> localVec(sendCounts[worldRank]);
    MPI_Scatterv(vec.data(), sendCounts.data(), displs.data(), MPI_INT, localVec.data(),
    sendCounts[worldRank], MPI_INT, 0, MPI_COMM_WORLD);

    std::pair<int, int> localMaxDiffPair = sequentialMostDifferentNeighborElements(localVec);

    std::pair<int, int> globalMaxDiffPair;
    MPI_Reduce(&localMaxDiffPair, &globalMaxDiffPair, 1, MPI_2INT, maxDiffOp, 0, MPI_COMM_WORLD);

    return globalMaxDiffPair;
}

std::vector<int> randomVector(int sizeVec, int minValue, int maxValue) {
    std::random_device rd;
    std::default_random_engine gen{rd()};
    std::uniform_int_distribution<> random(minValue, maxValue);

    std::vector<int> vec(sizeVec);

    for (int &value : vec)
        value = random(gen);

    return vec;
}
