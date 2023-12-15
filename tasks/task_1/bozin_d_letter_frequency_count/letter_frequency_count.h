// Copyright 2023 Bozin Dmitry
#ifndef TASKS_TASK_1_BOZIN_D_LETTER_FREQUENCY_COUNT_LETTER_FREQUENCY_COUNT_H_
#define TASKS_TASK_1_BOZIN_D_LETTER_FREQUENCY_COUNT_LETTER_FREQUENCY_COUNT_H_

#include <mpi.h>
#include <vector>
#include <string>

int calcFreqSeq(const std::string& str, char target);
int calcFreqPar(const std::string& str, char target);

#endif  // TASKS_TASK_1_BOZIN_D_LETTER_FREQUENCY_COUNT_LETTER_FREQUENCY_COUNT_H_