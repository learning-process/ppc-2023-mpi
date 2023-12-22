// Copyright 2023 Yurin Andrey

#include <random>
#include <algorithm>
#include <iterator>
#include <utility>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_3/yurin_a_shell_sort_simple_merge/shell_sort_simple_merge.h"

#define MAIN_PROCESS 0
#define DEFAULT_TAG 0

void CombineInOneProcess(
        const boost::mpi::communicator& world,
        std::vector<int>* array,
        const std::vector<int>& localArray,
        const std::vector<int>& sizes
) {
    boost::mpi::gatherv(world, localArray.data(), localArray.size(), array->data(), sizes, 0);

    if (world.rank() == 0) {
        int rightIndex = sizes[0] -1;
        for (int i = 1; i < sizes.size(); ++i) {
            int midIndex = rightIndex + 1;
            rightIndex += sizes[i];
            MergeTwoSortedArrays(array, 0, midIndex, rightIndex);
        }
    }
}

std::vector<int> RecalculateProcessesInvolved(std::vector<int> processesInvolved) {
    std::vector<int> newProcessesInvolved;

    for (auto iter = processesInvolved.begin(); iter != processesInvolved.end(); iter++) {
        if (std::distance(processesInvolved.begin(), iter) % 2 == 0) {
            if ((iter + 1) == processesInvolved.end()) {
                newProcessesInvolved.push_back(*(iter));
            }
        } else {
            newProcessesInvolved.push_back(*iter);
        }
    }
    return newProcessesInvolved;
}

void GetSortedArrayInNullProcess(
        const boost::mpi::communicator& world,
        std::vector<int>* array,
        const std::vector<int>& localArray
        ) {
    if (world.size() - 1 == 0)
        return;

    if (world.rank() == world.size() - 1) {
        world.send(MAIN_PROCESS, DEFAULT_TAG, localArray.data(), localArray.size());
    } else if (world.rank() == 0) {
        world.recv(world.size() - 1, DEFAULT_TAG, array->data(), array->size());
    }
    world.barrier();
}

void SendLocalArray(
        const boost::mpi::communicator& world,
        const std::vector<int>& localArray,
        const int& dest
        ) {
    world.send(dest, DEFAULT_TAG, localArray.size());
    world.send(dest, DEFAULT_TAG, localArray.data(), localArray.size());
}

void RecvLocalArrayAndSort(
        const boost::mpi::communicator& world,
        std::vector<int>* localArray,
        const int& dest
        ) {
    size_t sizeOfTempArray = 0;
    world.recv(dest, DEFAULT_TAG, sizeOfTempArray);
    std::vector<int> tempLocalArray(sizeOfTempArray);
    world.recv(dest, DEFAULT_TAG, tempLocalArray.data(), sizeOfTempArray);
    *localArray = MergeTwoSortedArrays(*localArray, tempLocalArray);
}

void OneStepForCombineByPair(
        const boost::mpi::communicator& world,
        std::vector<int>* localArray,
        const std::vector<int>& processesInvolved
        ) {
    auto iter = std::find(processesInvolved.begin(), processesInvolved.end(), world.rank());
    if (iter != processesInvolved.end()) {
        if (std::distance(processesInvolved.begin(), iter) % 2 == 0) {
            if ((iter + 1) != processesInvolved.end()) {
                SendLocalArray(world, *localArray, *(iter + 1));
            }
        } else {
            RecvLocalArrayAndSort(world, localArray, *(iter - 1));
        }
    }
}

void CombineByPair(
        const boost::mpi::communicator& world,
        std::vector<int>* array,
        std::vector<int> localArray
        ) {
    std::vector<int> processesInvolved;
    for (int i = 0; i < world.size(); ++i) {
        processesInvolved.push_back(i);
    }

    while (processesInvolved.size() > 1) {
        OneStepForCombineByPair(world, &localArray, processesInvolved);
        processesInvolved = RecalculateProcessesInvolved(processesInvolved);
        world.barrier();
    }

    GetSortedArrayInNullProcess(world, array, localArray);
}

bool IsSorted(std::vector<int> array) {
    for (int i = 0; i < array.size() - 1; ++i) {
        if (array[i] > array[i + 1]) {
            return false;
        }
    }
    return true;
}

std::vector<int> GetRandomVector(int size, int minValue, int maxValue) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<> distribution(minValue, maxValue);

    std::vector<int> vec(size);
    for (int  i = 0; i < size; i++) {
        vec[i] = distribution(gen);
    }
    return vec;
}

std::vector<int> MergeTwoSortedArrays(std::vector<int> firstArray, std::vector<int> secondArray) {
    std::vector<int> result(firstArray.size() + secondArray.size());

    for (int i = 0, j = 0; i + j < firstArray.size() + secondArray.size(); ) {
        if (i < firstArray.size() && j < secondArray.size()) {
            result[i + j - 1] = firstArray[i] < secondArray[j] ? firstArray[i++]: secondArray[j++];
        } else if (i < firstArray.size()) {
            result[i + j - 1] = firstArray[i++];
        } else {
            result[i + j - 1] = secondArray[j++];
        }
    }

    return result;
}

void MergeTwoSortedArrays(std::vector<int>* array, int leftIndex, int midIndex, int rightIndex) {
    std::vector<int> firstArray((*array).begin() + leftIndex, (*array).begin() + midIndex);
    std::vector<int> secondArray((*array).begin() + midIndex, (*array).begin() + rightIndex + 1);
    std::vector<int> result = MergeTwoSortedArrays(firstArray, secondArray);

    for (int i = 0; i < rightIndex - leftIndex + 1; ++i) {
        (*array)[leftIndex + i] = result[i];
    }
}

std::vector<int> ShellSortSequential(std::vector<int> array) {
    for (int step = array.size() / 2; step > 0; step /= 2) {
        for (int i = step; i < array.size(); ++i) {
            for (int j = i - step; j >= 0 && array[j] > array[j + step]; j -= step) {
                std::swap(array[j], array[j + step]);
            }
        }
    }

    return  array;
}

std::vector<int> ShellSortParallel(std::vector<int> array, bool combineInOneProcess) {
    boost::mpi::communicator world;
    int localSize = array.size() / world.size();
    std::vector<int> sizes(world.size(), localSize);

    if (array.size() % world.size()) {
        int elementsLeft = array.size() - world.size() * localSize;
        for (int i = 0; i < elementsLeft; ++i) {
            sizes[i]++;
        }
    }

    std::vector<int> localArray(sizes[world.rank()], 0);
    boost::mpi::scatterv(world, array.data(), sizes, localArray.data(), 0);
    localArray = ShellSortSequential(localArray);
    world.barrier();

    if (world.size() == 1) {
        return localArray;
    }

    if (combineInOneProcess) {
        CombineInOneProcess(world, &array, localArray, sizes);
    } else {
        CombineByPair(world, &array, localArray);
    }
    return array;
}
