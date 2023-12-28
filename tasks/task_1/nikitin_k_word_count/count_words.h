// Copyright 2023 Nikitin Kirill
#ifndef TASKS_TASK_1_NIKITIN_K_WORD_COUNT_COUNT_WORDS_H_
#define TASKS_TASK_1_NIKITIN_K_WORD_COUNT_COUNT_WORDS_H_

#include <mpi.h>

#include <string>
#include <ctime>
#include <sstream>
#include <iterator>

int countWords(std::string strLine, bool isWord);

int parallelCountWordsInString(const std::string& str);

std::string getRandString();

int getRandInt(int lBorder, int rBorder);

int sequentialCountWordsInString(const std::string& str);

#endif  // TASKS_TASK_1_NIKITIN_K_WORD_COUNT_COUNT_WORDS_H_
