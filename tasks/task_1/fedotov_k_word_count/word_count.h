// Copyright 2023 Fedotov Kirill
#ifndef TASKS_TASK_1_FEDOTOV_K_WORD_COUNT_WORD_COUNT_H_
#define TASKS_TASK_1_FEDOTOV_K_WORD_COUNT_WORD_COUNT_H_

#define EMPTY_STRING_ERROR -1

#include <string>
#include <vector>

void randWord(std::string* st, int add_size);
bool isLetter(char sym);
int getCount(std::string st);
int getLinearCount(std::string st, int size);

#endif  // TASKS_TASK_1_FEDOTOV_K_WORD_COUNT_WORD_COUNT_H_
