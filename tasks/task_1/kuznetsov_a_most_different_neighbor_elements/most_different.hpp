#ifndef _MOST_DIFFERENT_
#define _MOST_DIFFERENT_

#include <mpi.h>

#include <random>
#include <vector>

std::vector<int> createRandomArray(size_t size, int low, int up);
int sequentialFindMostDifferent(const std::vector<int>& arr);
int parallelFindMostDifferent(const std::vector<int>& arr);

#endif  // _MOST_DIFFERENT_
