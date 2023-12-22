// Copyright 2023 Tushentsova Karina
#ifndef TASKS_TASK_1_TUSHENTSOVA_K_COUNT_MIS_CHAR_COUNT_MIS_CHAR_H_
#define TASKS_TASK_1_TUSHENTSOVA_K_COUNT_MIS_CHAR_COUNT_MIS_CHAR_H_

#include <mpi.h>

#include <string>

int countNonMatchingChars(const std::string &s1, const std::string &s2);
int parCountNonMatchingChars(const std::string &s1, const std::string &s2);

#endif  // TASKS_TASK_1_TUSHENTSOVA_K_COUNT_MIS_CHAR_COUNT_MIS_CHAR_H_
