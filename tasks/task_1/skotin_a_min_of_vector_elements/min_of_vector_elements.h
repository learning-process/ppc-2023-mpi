// Copyright 2023 Khramov Ivan
#ifndef TASKS_TASK_1_SKOTIN_MIN_OF_VECTOR_ELEMENTS_H_
#define TASKS_TASK_1_SKOTIN_A_MIN_OF_VECTOR_ELEMENTS_H_

#include <vector>

class VectorMinFinder {
public:
    VectorMinFinder();
    ~VectorMinFinder();

    void initialize(int argc, char** argv);
    void finalize();
    int findMinParallel(const std::vector<int>& vector);
    static int findMinSequential(const std::vector<int>& vector);

private:
    int worldSize;
    int worldRank;
};

#endif  // TASKS_TASK_1_SKOTIN_MIN_OF_VECTOR_ELEMENTS_H_
