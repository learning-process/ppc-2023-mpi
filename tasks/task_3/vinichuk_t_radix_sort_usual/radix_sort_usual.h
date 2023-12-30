// Copyright 2023 Vinichuk Timofey
#ifndef TASKS_TASK_3_VINICHUK_T_RADIX_SORT_USUAL_RADIX_SORT_USUAL_H_
#define TASKS_TASK_3_VINICHUK_T_RADIX_SORT_USUAL_RADIX_SORT_USUAL_H_

#include <vector>

std::vector<int> getRandomVector(int  sz);
std::vector<int> SeqRadixSortUsualMerge(const std::vector<int>& arr);
std::vector<int> ParallelRadixSortUsualMerge(const std::vector<int>& arr);

#endif  // TASKS_TASK_3_VINICHUK_T_RADIX_SORT_USUAL_RADIX_SORT_USUAL_H_
