// Copyright 2023 Kosarev Egor
#include "task_2/kosarev_e_hypercube/hypercube.h"


int* generateRandomVector(int size, int seed) {
    if (size <= 0) {
        throw(-1);
    }

    int* randomVector = new int[size];

    std::mt19937 generator(seed);

    for (int i = 0; i < size; i++) {
        randomVector[i] = generator() % 100;
    }

    return randomVector;
}

bool checkHyperCube(int procNum) {
    if (procNum <= 0) {
        throw(-1);
    }

    while (procNum % 2 == 0) {
        procNum /= 2;
    }

    if (procNum == 1) {
        return true;
    }

    return false;
}

int calculateNextVertex(int sender, int dest) {
    if (sender == dest) {
        return sender;
    }

    int mask = 1;
    while (true) {
        if ((sender & mask) == (dest & mask)) {
            mask = mask << 1;
            continue;
        }

        if ((sender & mask) == 0) {
            return sender + mask;
        }
        else {
            return sender - mask;
        }
    }
}

void sendHyperCubeData(void* buffer, int count, MPI_Datatype dataType, int root,
    int dest, int tag, MPI_Comm communicator,
    std::vector<int>* pathToDest) {
    int ProcRank;
    MPI_Comm_rank(communicator, &ProcRank);

    if (root == dest) {
        if (ProcRank == 0) {
            pathToDest->push_back(root);
        }

        return;
    }

    int nextVertex = calculateNextVertex(root, dest);

    if (ProcRank == root) {
        MPI_Send(buffer, count, dataType, nextVertex, tag, communicator);
    }

    if (ProcRank == nextVertex) {
        MPI_Status status;
        MPI_Recv(buffer, count, dataType, root, tag, communicator, &status);
    }

    if (ProcRank == 0) {
        pathToDest->push_back(root);
    }

    sendHyperCubeData(buffer, count, dataType, nextVertex, dest, tag, communicator, pathToDest);
}
