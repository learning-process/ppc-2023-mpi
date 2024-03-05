// Copyright 2023 Fedotov Kirill
#ifndef TASKS_TASK_1_FEDOTOV_K_WORD_COUNT_WORD_COUNT_H_
#define TASKS_TASK_1_FEDOTOV_K_WORD_COUNT_WORD_COUNT_H_

#include <stdlib.h>
#include <mpi.h>
#include <vector>
#include <string>

std::string getRandString(int size);
int getWordsCountParallel(const std::string& str, int size);
int getWordsCountFragment(const std::string& str, int size);
int getWordsCountSequentially(const std::string& str, int size);

#endif  // TASKS_TASK_1_FEDOTOV_K_WORD_COUNT_WORD_COUNT_H_
