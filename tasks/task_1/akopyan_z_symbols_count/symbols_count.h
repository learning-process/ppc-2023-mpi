// Copyright 2023 Akopyan Zal
#ifndef TASKS_TASK_1_AKOPYAN_Z_SYMBOLS_COUNT_SYMBOLS_COUNT_H_
#define TASKS_TASK_1_AKOPYAN_Z_SYMBOLS_COUNT_SYMBOLS_COUNT_H_

#include <mpi.h>
#include <ctype.h>

#include <string>
#include <ctime>
#include <sstream>
#include <iterator>


int countSymbols(std::string line);

int parallelCountSymbolsInString(const std::string& str);

std::string getRandString();

int getRandInt(int leftBorder, int rightBorder);

int sequentialCountSymbolsInString(const std::string& str);

#endif  // TASKS_TASK_1_AKOPYAN_Z_SYMBOLS_COUNT_SYMBOLS_COUNT_H_
