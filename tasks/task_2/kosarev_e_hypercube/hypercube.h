// Copyright 2023 Kosarev Egor
#ifndef TASKS_TASK_2_KOSAREV_E_HYPERCUBE_HYPERCUBE_H_
#define TASKS_TASK_2_KOSAREV_E_HYPERCUBE_HYPERCUBE_H_

#include <mpi.h>

#include <vector>
#include <random>

int* getRandomVector(int size, int seed);

bool isHyperCube(int procNum);

void sendHyperCube(void* buf, int count, MPI_Datatype datatype, int root,
    int dest, int tag, MPI_Comm comm, std::vector<int>* pathToDest);

#endif  // TASKS_TASK_2_KOSAREV_E_HYPERCUBE_HYPERCUBE_H_
