// Copyright 2023 Kosarev Egor
#include "task_2/kosarev_e_hypercube/hypercube.h"

int* getRandomVector(int size, int seed) {
    if (size <= 0) {
        throw(-1);
    }

    int* randomVector = new int[size];

    std::mt19937 gen(seed);

    for (int i = 0; i < size; i++) {
        randomVector[i] = gen() % 100;
    }

    return randomVector;
}

bool isHyperCube(int procNum) {
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

int getNextVert(int sender, int dest) {
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

void sendHyperCube(void* buf, int count, MPI_Datatype datatype, int root,
    int dest, int tag, MPI_Comm comm,
    std::vector<int>* pathToDest) {
    int procRank;
    MPI_Comm_rank(comm, &procRank);

    if (root == dest) {
        if (procRank == 0) {
            pathToDest->push_back(root);
        }

        return;
    }

    int nextVert = getNextVert(root, dest);

    if (procRank == root) {
        MPI_Send(buf, count, datatype, nextVert, tag, comm);
    }

    if (procRank == nextVert) {
        MPI_Status status;
        MPI_Recv(buf, count, datatype, root, tag, comm, &status);
    }

    if (procRank == 0) {
        pathToDest->push_back(root);
    }

    sendHyperCube(buf, count, datatype, nextVert, dest, tag, comm, pathToDest);
}
