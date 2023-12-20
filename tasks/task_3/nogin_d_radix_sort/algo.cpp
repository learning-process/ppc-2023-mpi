// Copyright 2023 Nogin Denis

#include "task_3/nogin_d_radix_sort/algo.h"

std::vector<int> randomVector(int sizeVec, int minValue, int maxValue) {
    std::random_device rd;
    std::default_random_engine gen{rd()};
    std::uniform_int_distribution<> random(minValue, maxValue);

    std::vector<int> vec(sizeVec);

    for (int &value : vec)
        value = random(gen);

    return vec;
}

std::vector<int> merge(const std::vector<int>& vecLeft, const std::vector<int>& vecRight) {
    std::vector<int> result((vecLeft.size() + vecRight.size()));
    int i = 0, j = 0, k = 0;

    while ((i < vecLeft.size()) && (j < vecRight.size())) {
        if (vecLeft[i] < vecRight[j])
            result[k] = vecLeft[i++];
        else
            result[k] = vecRight[j++];
        k++;
    }

    while (i < vecLeft.size()) {
        result[k++] = vecLeft[i++];
    }

    while (j < vecRight.size()) {
        result[k++] = vecRight[j++];
    }

    return result;
}

int getNumber(int num, int rad) {
    if (rad > 0) {
        int mask = pow(10, rad);
        int tmp = num / mask;

        return tmp % 10;
    }

    return  static_cast<int>(num * pow(10, (-rad))) % 10;
}

int right(int num) {
    std::ostringstream strs;
    strs << num;
    std::string str = strs.str();

    if (str.find('.')) {
        return -((str.find('.')) - (str.size())) - 1;
    } else {
        return 0;
    }
}

int left(int num) {
    int numRadix = 0;

    while (num > 1) {
        num /= 10;
        numRadix++;
    }

    return numRadix;
}

std::vector<int> radixStep(const std::vector<int>& vec, int rad) {
    std::vector<int> result;
    std::vector <std::vector<int>> radix(10);

    for (int i = 0; i < vec.size(); ++i) {
        radix[getNumber(vec[i], rad)].push_back(vec[i]);
    }

    for (int i = 0; i < radix.size(); ++i) {
        for (int j = 0; j < radix[i].size(); ++j) {
            result.push_back(radix[i][j]);
        }
    }

    return result;
}

std::vector<int> radixSort(const std::vector<int>& vec) {
    std::vector<int> result;
    if (vec.size() < 2)
        return result;

    result = vec;

    int radixNegative = 0;
    int maxRadixNegative = right(vec[0]);

    for (int i = 1; i < vec.size(); ++i) {
        radixNegative = right(vec[i]);
        if (maxRadixNegative < radixNegative) {
            maxRadixNegative = radixNegative;
        }
    }

    int max = vec[0];
    for (int i = 1; i < vec.size(); ++i) {
        if (vec[i] > max) {
            max = vec[i];
        }
    }

    int maxRadixPositive = left(max);

    for (int i = -maxRadixNegative; i <= maxRadixPositive; ++i) {
        result = radixStep(result, i);
    }

    return result;
}

std::vector<int> parallelRadixSort(const std::vector<int>& vec) {
    int worldSize;
    int worldRank;
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    std::vector<int> result;
    if (vec.size() < 2)
        return result;

    int size = vec.size();
    int count;

    std::vector<int> sendCounts(worldSize);
    std::vector<int> displs(worldSize);
    displs[0] = 0;

    if ((worldRank >= size % worldSize)) {
        count = size / worldSize;
    } else {
        count = (size / worldSize) + 1;
    }

    std::vector<int> recvbuf(count);

    if (recvbuf.size() == 0) {
        recvbuf.resize(1);
    }

    for (int i = 0; i < worldSize; ++i) {
        if (i < (size % worldSize)) {
            sendCounts[i] = (size / worldSize) + 1;
        } else {
            sendCounts[i] = size / worldSize;
        }

        if (i > 0) {
            displs[i] = (displs[i - 1] + sendCounts[i - 1]);
        }
    }

    MPI_Scatterv(vec.data(), sendCounts.data(), displs.data(), MPI_INT, recvbuf.data(),
        count, MPI_INT, 0, MPI_COMM_WORLD);

    recvbuf = radixSort(recvbuf);
    if (worldRank == 0) {
        result = recvbuf;
        MPI_Status status;

        for (int i = 1; i < worldSize; ++i) {
            recvbuf.resize(sendCounts[i]);
            MPI_Recv(recvbuf.data(), sendCounts[i], MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            result = merge(result, recvbuf);
        }
    } else {
        MPI_Send(recvbuf.data(), count, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    return result;
}
