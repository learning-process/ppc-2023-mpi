// Copyright 2023 Kriseev Mikhail

#include "task_2/kriseev_m_gauss_jordan_method/gauss_jordan_method.h"
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
            bool zeroRow = true;
            for (int l = 0; l < matrixSize; ++l) {
                if (systemMatrix[i * matrixSize + l] != 0) {
                    zeroRow = false;
                    break;
                }
            }
            if (zeroRow && solutionVector[i] != 0) {
                throw std::invalid_argument("unknownsMatrix");
            }
        }
    }
    return solutionVector;
}

std::vector<double> gaussJordanMethodParallel(
    const std::vector<double> &unknownsMatrix,
    const std::vector<double> &constants) {
    //
    boost::mpi::communicator world;
    if (world.size() == 1) {
        return gaussJordanMethodSequential(unknownsMatrix, constants);
    }
    int matrixSize = constants.size();
    auto systemMatrix = std::vector<double>(unknownsMatrix);
    auto solutionVector = std::vector<double>(constants);
    auto oldPivotRows = std::vector<double>();

    int extraWork = matrixSize % world.size();
    int work = matrixSize / world.size();
    std::vector<int> sizes_(world.size(), work * matrixSize);
    std::vector<int> solutionSizes_(world.size(), work);
    std::vector<size_t> beginIndices(world.size(), 0);
    for (int i = 0; i < extraWork; ++i) {
        solutionSizes_[i]++;
        sizes_[i] += matrixSize;
    }
    const std::vector<int> sizes(sizes_);
    const std::vector<int> solutionSizes(solutionSizes_);

    for (int i = 1; i < beginIndices.size(); ++i) {
        beginIndices[i] = beginIndices[i - 1] + solutionSizes[i - 1];
    }

    auto myWork = solutionSizes[world.rank()];
    auto myBeginIndex = beginIndices[world.rank()];

    for (int k = 0; k < matrixSize; ++k) {
        int pivotRow = 0;
        if (world.rank() == 0) {
            while (pivotRow < matrixSize &&
                   (!systemMatrix[pivotRow * matrixSize + k] ||
                    std::find(oldPivotRows.begin(), oldPivotRows.end(),
                              pivotRow) != oldPivotRows.end())) {
                pivotRow++;
            }
            bool flag = pivotRow == matrixSize;
            boost::mpi::broadcast(world, flag, 0);
            if (flag) {
                continue;
            }
            boost::mpi::broadcast(world, pivotRow, 0);
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
            boost::mpi::broadcast(world, solutionVector.data(),
                                  solutionVector.size(), 0);
        } else {
            bool flag = false;
            boost::mpi::broadcast(world, flag, 0);
            if (flag) {
                continue;
            }
            boost::mpi::broadcast(world, pivotRow, 0);
            boost::mpi::broadcast(world,
                                  systemMatrix.data() + pivotRow * matrixSize,
                                  matrixSize, 0);
            boost::mpi::broadcast(world, solutionVector.data(),
                                  solutionVector.size(), 0);
        }
        for (int i = myBeginIndex; i < myBeginIndex + myWork; ++i) {
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
        std::vector<double> solCopy(myWork);
        std::vector<double> rowsCopy(myWork * matrixSize);
        for (int i = 0; i < myWork; ++i) {
            solCopy[i] = solutionVector[myBeginIndex + i];
        }
        for (int i = 0; i < rowsCopy.size(); ++i) {
            rowsCopy[i] = systemMatrix[i + myBeginIndex * matrixSize];
        }
        boost::mpi::all_gatherv(world, solutionVector.data() + myBeginIndex,
                                solutionVector.data(), solutionSizes);
        boost::mpi::all_gatherv(world,
                                systemMatrix.data() + myBeginIndex * matrixSize,
                                systemMatrix.data(), sizes);
    }

    for (int i = 0; i < matrixSize; ++i) {
        bool flag = true;
        if (solutionVector[i] == 0) {
            flag = false;
        }
        for (int j = 0; flag && j < matrixSize; j++) {
            if (systemMatrix[matrixSize * i + j] != 0) {
                flag = false;
            }
        }
        if (flag) {
            throw std::invalid_argument("unknownsMatrix");
        }
    }
    return solutionVector;
}
