// Copyright 2023 Kulikov Artem
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <functional>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>
#include "task_2/lesnikov_hypercube/hypercube.h"


bool is2Degree(int n) {
    if (!n) {
        return false;
    }

    int copy_n = n;

    while (copy_n % 2 == 0 && copy_n) {
        copy_n /= 2;
    }

    return !static_cast<bool>(copy_n);
}

void writeIdentity(std::vector<std::vector<bool>>* m, int start_i, int end_i, int start_j) {
    for (int i = start_i, j = start_j; i < end_i; i++, j++) {
        (*m)[i][j] = true;
    }
}

void fillMatrix(std::vector<std::vector<bool>>* m, int start, int end) {
    writeIdentity(m, start, start + (end - start) / 2, start + (end - start) / 2);
    writeIdentity(m, start + (end - start) / 2, end, start);

    if (end - start != 2) {
        fillMatrix(m, start, start + (end - start) / 2);
        fillMatrix(m, start + (end - start) / 2, end);
    }
}

std::vector<std::vector<bool>> createHypercube(int n) {
    if (n == 0) {
        throw std::invalid_argument("hypercube size cannot be 0");
    } else if (!is2Degree(n)) {
        throw std::invalid_argument("hypercube size must be 2 power");
    }

    std::vector<std::vector<bool>> m(n);

    for (auto& vec : m) {
        vec = std::vector<bool>(n);
    }

    fillMatrix(&m, 0, n);

    return m;
}

std::queue<int> getPath(const std::vector<std::vector<bool>>& m, int start, int end) {
    std::unordered_set<int> labelled;

    std::queue<std::pair<int, std::queue<int>>> verticiesAndPath;

    verticiesAndPath.push(std::make_pair(start, std::queue<int>()));
    labelled.insert(start);

    while (!verticiesAndPath.empty()) {
        int size = verticiesAndPath.size();

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < m[0].size(); j++) {
                if (m[verticiesAndPath.front().first][j] && labelled.find(j) == labelled.end()) {
                    labelled.insert(j);
                    std::queue<int> newPath(verticiesAndPath.front().second);
                    newPath.push(verticiesAndPath.front().first);

                    if (j == end) {
                        newPath.push(end);
                        return newPath;
                    }

                    verticiesAndPath.push(std::make_pair(j, newPath));
                }
            }

            verticiesAndPath.pop();
        }
    }

    return std::queue<int>();
}

std::pair<std::vector<int>, std::vector<int>>
getTransitionsAndExpectations(std::queue<int> path, int numProcess) {
    std::vector<int> transitions(numProcess, -1);
    std::vector<int> expectations(numProcess, -1);

    int prev;
    int cur;

    if (!path.empty()) {
        prev = path.front();
        path.pop();
    }

    while (!path.empty()) {
        cur = path.front();
        path.pop();
        transitions[prev] = cur;
        expectations[cur] = prev;
        prev = cur;
    }

    return std::make_pair(transitions, expectations);
}
