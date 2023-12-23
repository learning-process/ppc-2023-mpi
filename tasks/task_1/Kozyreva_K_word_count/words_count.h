// Copyright 2023 Kozyreva K
#ifndef TASKS_TASK_1_KOZYREVA_K_WORD_COUNT_WORDS_COUNT_H_
#define TASKS_TASK_1_KOZYREVA_K_WORD_COUNT_WORDS_COUNT_H_

#define EMPTY_STRING_ERROR -1

#include <string>

int SimpleCount(const std::string);
int ParallelCount(const std::string);
int DeltaProcessCount(const std::string);
std::string getLongString(int size);

#endif  // TASKS_TASK_1_KOZYREVA_K_WORD_COUNT_WORDS_COUNT_H_
