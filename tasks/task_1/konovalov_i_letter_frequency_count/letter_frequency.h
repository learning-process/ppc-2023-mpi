// Copyright 2023 Konovalov Igor
#ifndef TASKS_TASK_1_KONOVALOV_I_LETTER_FREQUENCY_H_
#define TASKS_TASK_1_KONOVALOV_I_LETTER_FREQUENCY_H_

#include <mpi.h>
#include <vector>

double letterFreqCalcPar(const char* str, char l);
int letterCountInChunk(const char* str, char l);
double letterFreqCalcSeq(const char* str, char l);

#endif //TASKS_TASK_1_KONOVALOV_I_LETTER_FREQUENCY_CALCULATION_H_