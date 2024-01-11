// Copyright 2023 Lebedeva Ksenia
#ifndef TASKS_TASK_1_LEBEDEVA_K_WORDS_COUNT_WORDS_COUNT_H_
#define TASKS_TASK_1_LEBEDEVA_K_WORDS_COUNT_WORDS_COUNT_H_

#include <stdlib.h>
#include <mpi.h>
#include <vector>
#include <string>

std::string getRandString(int size);
int getWordsCountParallel(const std::string& str, int size);
int getWordsCountFragment(const std::string& str, int size);
int getWordsCountSequentially(const std::string& str, int size);

#endif  // TASKS_TASK_1_LEBEDEVA_K_WORDS_COUNT_WORDS_COUNT_H_
