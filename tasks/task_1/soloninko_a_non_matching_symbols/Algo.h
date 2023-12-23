// Copyright 2023 Soloninko Andrey
#ifndef TASKS_TASK_1_SOLONINKO_A_NON_MATCHING_SYMBOLS_ALGO_H_
#define TASKS_TASK_1_SOLONINKO_A_NON_MATCHING_SYMBOLS_ALGO_H_

#include <mpi.h>

#include <random>
#include <string>

int GetStringDifDef(const std::string &s1, const std::string &s2);
int GetStringDifPar(const std::string &s1, const std::string &s2);
std::string random_string(int length);

#endif  // TASKS_TASK_1_SOLONINKO_A_NON_MATCHING_SYMBOLS_ALGO_H_
