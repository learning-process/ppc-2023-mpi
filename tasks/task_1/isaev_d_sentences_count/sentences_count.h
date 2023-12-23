// Copyright 2023 Isaev Dmitriy
#ifndef TASKS_TASK_1_ISAEV_D_SENTENCES_COUNT_SENTENCES_COUNT_H_
#define TASKS_TASK_1_ISAEV_D_SENTENCES_COUNT_SENTENCES_COUNT_H_

#include <mpi.h>
#include <vector>
#include <string>

int countSequence(const std::string &str);
int countParallel(const std::string &str);

#endif  // TASKS_TASK_1_ISAEV_D_SENTENCES_COUNT_SENTENCES_COUNT_H_
