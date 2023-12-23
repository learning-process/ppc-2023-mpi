// Copyright 2023 Alexseev Danila
#ifndef TASKS_TASK_1_ALEXSEEV_D_WORD_COUNT_COUNT_WORDS_H_
#define TASKS_TASK_1_ALEXSEEV_D_WORD_COUNT_COUNT_WORDS_H_

#include <mpi.h>

#include <string>
#include <ctime>
#include <sstream>
#include <iterator>

int countWords(std::string line, bool isWord);

int parallelCountWordsInString(const std::string& str);

std::string getRandString();

int getRandInt(int leftBorder, int rightBorder);

int sequentialCountWordsInString(const std::string& str);

#endif  // TASKS_TASK_1_ALEXSEEV_D_WORD_COUNT_COUNT_WORDS_H_
