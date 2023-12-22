// Copyright 2023 Kulikov Artem
#ifndef TASKS_TASK_2_LESNIKOV_HYPERCUBE_HYPERCUBE_H_
#define TASKS_TASK_2_LESNIKOV_HYPERCUBE_HYPERCUBE_H_

#include <utility>
#include <algorithm>
#include <vector>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <exception>
#include <boost/serialization/vector.hpp>


bool is2Degree(int n);
void writeIdentity(std::vector<std::vector<bool>>* m, int start_i, int end_i, int start_j);
void fillMatrix(std::vector<std::vector<bool>>* m, int start, int end);
std::vector<std::vector<bool>> createHypercube(int n);
std::queue<int> getPath(const std::vector<std::vector<bool>>& m, int start, int end);
std::pair<std::vector<int>, std::vector<int>> getTransitionsAndExpectations(std::queue<int> path, int numProcess);

template<class T>
void sendRecvData(int source, int dest, int tag, T* value) {
    boost::mpi::communicator world;

    if (source < 0 || source >= world.size() || dest < 0 || dest >= world.size() || source == dest) {
        throw std::invalid_argument("invalid source or dest");
    }

    std::vector<std::vector<bool>> hypercube = createHypercube(world.size());
    std::pair<std::vector<int>, std::vector<int>> transitionsAndExpectations =
    getTransitionsAndExpectations(getPath(hypercube, source, dest), world.size());

    T data;

    if (world.rank() != source && world.rank() != dest) {
        if (transitionsAndExpectations.second[world.rank()] != -1) {
            world.recv(transitionsAndExpectations.second[world.rank()], tag, data);
        }
        if (transitionsAndExpectations.second[world.rank()] != -1) {
            world.send(transitionsAndExpectations.first[world.rank()], tag, data);
        }
    } else if (world.rank() == source) {
        world.send(transitionsAndExpectations.first[world.rank()], tag, *value);
    } else {
        world.recv(transitionsAndExpectations.second[world.rank()], tag, data);
    }

    if (world.rank() == dest) {
        *value = data;
    }
}

#endif  // TASKS_TASK_2_LESNIKOV_HYPERCUBE_HYPERCUBE_H_
