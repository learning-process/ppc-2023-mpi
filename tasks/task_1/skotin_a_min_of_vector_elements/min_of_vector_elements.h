// Copyright 2023 Skotin Alexander
#ifndef TASKS_TASK_1_SKOTIN_A_MIN_OF_VECTOR_ELEMENTS_MIN_OF_VECTOR_ELEMENTS_H_
#define TASKS_TASK_1_SKOTIN_A_MIN_OF_VECTOR_ELEMENTS_MIN_OF_VECTOR_ELEMENTS_H_

#include <vector>
#include <utility>

class VectorMinFinder {
 public:
    VectorMinFinder() = default;
    ~VectorMinFinder() = default;

    int parallelMin(const std::vector<int>& data);
    static int sequentialMin(const std::vector<int>& data);
    static std::vector<int> generateRandomVector(int size, int min = -100, int max = 100);
};

#endif  // TASKS_TASK_1_SKOTIN_A_MIN_OF_VECTOR_ELEMENTS_MIN_OF_VECTOR_ELEMENTS_H_
