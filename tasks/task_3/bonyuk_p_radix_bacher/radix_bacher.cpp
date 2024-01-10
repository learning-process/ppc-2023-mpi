//  Copyright 2023 Bonyuk Petr

#include <mpi.h>

#include <algorithm>
#include <cmath>
#include <ctime>
#include <random>
#include <iostream>
#include <utility>
#include <vector>
#include <functional>

#include "task_3/bonyuk_p_radix_bacher/radix_bacher.h"

std::vector<int> radixSort(std::vector<int> nums) {
    if (nums.empty()) {
        return nums;
    }
    int maxNum = *std::max_element(nums.begin(), nums.end());
    int minNum = *std::min_element(nums.begin(), nums.end());

    std::vector<int> negatives, positives;
    for (int num : nums) {
        if (num < 0)
            negatives.push_back(-num);
        else
            positives.push_back(num);
    }

    positives = radixSortUnsigned(positives);
    negatives = radixSortUnsigned(negatives);
    std::transform(negatives.begin(), negatives.end(), negatives.begin(), std::negate<int>());
    std::reverse(negatives.begin(), negatives.end());
    nums.clear();
    nums.insert(nums.end(), negatives.begin(), negatives.end());
    nums.insert(nums.end(), positives.begin(), positives.end());
    return nums;
}

std::vector<int> radixSortUnsigned(std::vector<int> nums) {
    std::vector<int> output(nums.size());
    int maxNum = *std::max_element(nums.begin(), nums.end());
    int exp = 1;
    while (maxNum / exp > 0) {
        std::vector<int> count(10, 0);

        for (int num : nums) {
            count[(num / exp) % 10]++;
        }

        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        for (int i = nums.size() - 1; i >= 0; i--) {
            output[count[(nums[i] / exp) % 10] - 1] = nums[i];
            count[(nums[i] / exp) % 10]--;
        }

        for (int i = 0; i < nums.size(); i++) {
            nums[i] = output[i];
        }

        exp *= 10;
    }
    return nums;
}

int checkMPIResult(int result) {
    if (result != MPI_SUCCESS) {
        char errorString[BUFSIZ];
        int lengthOfErrorString, errorClass;
        MPI_Error_class(result, &errorClass);
        MPI_Error_string(errorClass, errorString, &lengthOfErrorString);
        std::cerr << "MPI Error: " << errorString << std::endl;
        MPI_Abort(MPI_COMM_WORLD, result);
        exit(result);
    }
    return result;
}
void compareExchange(std::vector<int>* local_nums, int i, int j, int dir) {
    if (dir == ((*local_nums)[i] > (*local_nums)[j])) {
        std::swap((*local_nums)[i], (*local_nums)[j]);
    }
}

void batcherMerge(std::vector<int>* local_nums, int numProcs, int myRank) {
    int local_size = local_nums->size();
    int n = local_size * numProcs;
    int t = log2(n);
    int p = 1 << (t - 1);

    while (p > 0) {
        int q = 1 << (t - 1);
        int r = 0;
        int d = p;
        while (d > 0) {
            for (int i = 0; i < n - d; ++i) {
                if ((i & p) == r) {
                    int idx1 = i / local_size;
                    int idx2 = (i + d) / local_size;
                    int local_idx1 = i % local_size;
                    int local_idx2 = (i + d) % local_size;

                    if (myRank == idx1 || myRank == idx2) {
                        if (idx1 != idx2) {
                            int partner = myRank == idx1 ? idx2 : idx1;
                            int local_value = myRank == idx1 ? (*local_nums)[local_idx1] : (*local_nums)[local_idx2];
                            int remote_value;
                            checkMPIResult(MPI_Sendrecv(&local_value, 1, MPI_INT, partner, 0,
                                &remote_value, 1, MPI_INT, partner, 0,
                                MPI_COMM_WORLD, MPI_STATUS_IGNORE));
                            bool needSwap = (myRank < partner) ? (local_value > remote_value) :
                                (local_value < remote_value);
                            if (needSwap) {
                                if (myRank == idx1) {
                                    (*local_nums)[local_idx1] = remote_value;
                                } else {
                                    (*local_nums)[local_idx2] = remote_value;
                                }
                            }
                        } else {
                            if (local_idx1 < local_idx2 && (*local_nums)[local_idx1] >(*local_nums)[local_idx2]) {
                               std::swap((*local_nums)[local_idx1], (*local_nums)[local_idx2]);
                            }
                        }
                    }
                }
            }
            if (q != p) {
                d = q - p;
                r = p;
                q >>= 1;
            } else {
                d = 0;
            }
        }
        p >>= 1;
    }
}
