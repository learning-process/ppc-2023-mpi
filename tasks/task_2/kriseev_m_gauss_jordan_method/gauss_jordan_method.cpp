// Copyright 2023 Kriseev Mikhail

#include "task_2/kriseev_m_gauss_jordan_method/gauss_jordan_method.h"
#include <iostream>
#include <algorithm>
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
        if (pivotRow == matrixSize) {
            continue;
        }
        oldPivotRows.emplace_back(pivotRow);
        for (int j = k + 1; j < matrixSize; ++j) {
            systemMatrix[pivotRow * matrixSize + j] /=
                systemMatrix[pivotRow * matrixSize + k];
        }
        solutionVector[pivotRow] /= systemMatrix[pivotRow * matrixSize + k];
        systemMatrix[pivotRow * matrixSize + k] = 1.0;
        for (int i = 0; i < matrixSize; ++i) {
            if (pivotRow == i) {
                continue;
            }
            double oldFirst = systemMatrix[i * matrixSize + k];
            for (int j = k; j < matrixSize; ++j) {
                systemMatrix[i * matrixSize + j] -=
                    systemMatrix[pivotRow * matrixSize + j] * oldFirst;
            }
            solutionVector[i] -= solutionVector[pivotRow] * oldFirst;
        }
    }
    return solutionVector;
}

void printMatrix(int matrixSize, const std::vector<double> &systemMatrix,
                 const std::vector<double> &solutionVector) {
    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
        }
    }
}

std::vector<double> gaussJordanMethodParallel(
    const std::vector<double> &unknownsMatrix,
    const std::vector<double> &constants) {
    //
    boost::mpi::communicator world;
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
                if (i == pivotRow) {
                    continue;
                }
                int receiverRank = i % (world.size() - 1) + 1;
                world.send(receiverRank, 0, i);
                world.send(receiverRank, 1,
                           systemMatrix.data() + i * matrixSize, matrixSize);
                double oldFirst = systemMatrix[i * matrixSize + k];
                solutionVector[i] -= solutionVector[pivotRow] * oldFirst;
            }
            for (int i = 1; i < world.size(); ++i) {
                world.send(i, 2);  // send terminator message
            }
            int finishedCount = 0;
            while (finishedCount < world.size() - 1) {
                status = world.iprobe();
                if (!status.has_value()) {
                    continue;
                }
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
            int count = 0;
            while (1) {
                status = world.iprobe(0);
                if (!status.has_value()) {
                    continue;
                }
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
    return solutionVector;
}
