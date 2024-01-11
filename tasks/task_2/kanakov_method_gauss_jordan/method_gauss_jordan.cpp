// Copyright 2023 Kanakov Roman
#include "task_2/kanakov_method_gauss_jordan/method_gauss_jordan.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/status.hpp>

std::vector<double> gaussJordanMethodSequential(
    const std::vector<double> &unknownsMatrix,
    const std::vector<double> &constants) {
    int matrixSize = constants.size();
    auto systemMatrix = std::vector<double>(unknownsMatrix);
    auto solutionVector = std::vector<double>(constants);
    auto oldPivotRows = std::vector<double>();

    for (int k = 0; k < matrixSize; ++k) {
        int pivotRow = 0;
        while (pivotRow < matrixSize &&
               (!systemMatrix[pivotRow * matrixSize + k] ||
                std::find(oldPivotRows.begin(), oldPivotRows.end(), pivotRow) !=
                    oldPivotRows.end())) {
            pivotRow++;
        }
        if (pivotRow == matrixSize)
            continue;
        oldPivotRows.emplace_back(pivotRow);
        for (int j = k + 1; j < matrixSize; ++j) {
            systemMatrix[pivotRow * matrixSize + j] /=
                systemMatrix[pivotRow * matrixSize + k];
        }
        solutionVector[pivotRow] /= systemMatrix[pivotRow * matrixSize + k];
        systemMatrix[pivotRow * matrixSize + k] = 1.0;
        for (int i = 0; i < matrixSize; ++i) {
            if (pivotRow == i)
                continue;
            double oldFirst = systemMatrix[i * matrixSize + k];
            for (int j = k; j < matrixSize; ++j) {
                systemMatrix[i * matrixSize + j] -=
                    systemMatrix[pivotRow * matrixSize + j] * oldFirst;
            }

            solutionVector[i] -= solutionVector[pivotRow] * oldFirst;
            bool zeroRow = true;
            for (int l = 0; l < matrixSize; ++l) {
                if (systemMatrix[i * matrixSize + l] != 0) {
                    zeroRow = false;
                    break;
                }
            }
            if (zeroRow && solutionVector[i] != 0)
                throw std::invalid_argument("unknownsMatrix");
        }
    }
    return solutionVector;
}

std::vector<double> gaussJordanMethodParallel(
    const std::vector<double> &unknownsMatrix,
    const std::vector<double> &constants) {
    boost::mpi::communicator world;
    if (world.size() == 1)
        return gaussJordanMethodSequential(unknownsMatrix, constants);
    int matrixSize = constants.size();
    auto systemMatrix = std::vector<double>(unknownsMatrix);
    auto solutionVector = std::vector<double>(constants);
    auto oldPivotRows = std::vector<double>();

    std::vector<double> pivotRowVector(matrixSize);
    std::vector<double> tempRowVector(matrixSize);
    boost::optional<boost::mpi::status> status;

    for (int k = 0; k < matrixSize; ++k) {
        int pivotRow = 0;
        if (world.rank() == 0) {
            while (pivotRow < matrixSize &&
                   (!systemMatrix[pivotRow * matrixSize + k] ||
                    std::find(oldPivotRows.begin(), oldPivotRows.end(),
                              pivotRow) != oldPivotRows.end())) {
                pivotRow++;
            }
            if (pivotRow == matrixSize) {
                boost::mpi::broadcast(world, tempRowVector.data(), matrixSize,
                                      0);
                std::vector<boost::mpi::request> op(world.size() - 1);
                for (int i = 1; i < world.size(); ++i) {
                    world.send(i, 2);
                    op[i - 1] = world.irecv(i, 3);
                }
                boost::mpi::wait_all(op.begin(), op.end());
                continue;
            }
            oldPivotRows.emplace_back(pivotRow);
            for (int j = k + 1; j < matrixSize; ++j) {
                systemMatrix[pivotRow * matrixSize + j] /=
                    systemMatrix[pivotRow * matrixSize + k];
            }
            solutionVector[pivotRow] /= systemMatrix[pivotRow * matrixSize + k];
            systemMatrix[pivotRow * matrixSize + k] = 1.0;

            boost::mpi::broadcast(world,
                                  systemMatrix.data() + pivotRow * matrixSize,
                                  matrixSize, 0);

            for (int i = 0; i < matrixSize; ++i) {
                if (i == pivotRow)
                    continue;
                int receiverRank = i % (world.size() - 1) + 1;
                world.send(receiverRank, 0, i);
                world.send(receiverRank, 1,
                           systemMatrix.data() + i * matrixSize, matrixSize);
                double oldFirst = systemMatrix[i * matrixSize + k];
                solutionVector[i] -= solutionVector[pivotRow] * oldFirst;
            }
            for (int i = 1; i < world.size(); ++i) {
                world.send(i, 2);
            }
            int finishedCount = 0;
            while (finishedCount < world.size() - 1) {
                status = world.iprobe();
                if (!status.has_value())
                    continue;
                if (status.value().tag() == 3) {
                    world.recv(status.value().source(), 3);
                    finishedCount++;
                    continue;
                }
                int rowIndex = -1;
                world.recv(status.value().source(), 0, rowIndex);
                world.recv(status.value().source(), 1,
                           systemMatrix.data() + matrixSize * rowIndex,
                           matrixSize);
            }
        } else {
            boost::mpi::broadcast(world, pivotRowVector.data(), matrixSize, 0);
            while (1) {
                status = world.iprobe(0);
                if (!status.has_value())
                    continue;
                if (status.value().tag() == 2) {
                    world.recv(0, 2);
                    break;
                }
                int index = -1;
                world.recv(0, 0, index);
                world.recv(0, 1, tempRowVector.data(), matrixSize);
                double oldFirst = tempRowVector[k];
                for (int i = 0; i < matrixSize; ++i) {
                    tempRowVector[i] -= pivotRowVector[i] * oldFirst;
                }
                world.send(0, 0, index);
                world.send(0, 1, tempRowVector.data(), matrixSize);
            }
            world.send(0, 3);
        }
    }
    if (world.rank() == 0) {
        for (int i = 0; i < matrixSize; ++i) {
            bool flag = true;
            if (solutionVector[i] == 0)
                flag = false;
            for (int j = 0; flag && j < matrixSize; j++) {
                if (systemMatrix[matrixSize * i + j] != 0)
                    flag = false;
            }
            if (flag) {
                int f = 1;
                boost::mpi::broadcast(world, f, 0);
                throw std::invalid_argument("unknownsMatrix");
            }
        }
        int f = 0;
        boost::mpi::broadcast(world, f, 0);
    } else {
        int f = 0;
        boost::mpi::broadcast(world, f, 0);
        if (f == 1) {
            throw std::invalid_argument("unknownsMatrix");
        }
    }
    return solutionVector;
}
