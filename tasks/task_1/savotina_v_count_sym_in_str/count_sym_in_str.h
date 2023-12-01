// Copyright 2023 Savotina Valeria
#ifndef TASKS_TASK_1_SAVOTINA_V_COUNT_SYM_IN_STR_COUNT_SYM_IN_STR_H_
#define TASKS_TASK_1_SAVOTINA_V_COUNT_SYM_IN_STR_COUNT_SYM_IN_STR_H_

#include <mpi.h>

#include <algorithm>
#include <limits>
#include <random>
#include <string>
#include <vector>

template <typename T>
using num_lim = std::numeric_limits<T>;

int seqCountSymInString(const std::string& str, char sym);
int parCountSymInString(const std::string& str, const char sym, MPI_Comm comm);
std::string createRandomString(size_t size, char minSym = num_lim<char>::min(),
                               char maxSym = num_lim<char>::max());

#endif  // TASKS_TASK_1_SAVOTINA_V_COUNT_SYM_IN_STR_COUNT_SYM_IN_STR_H_
