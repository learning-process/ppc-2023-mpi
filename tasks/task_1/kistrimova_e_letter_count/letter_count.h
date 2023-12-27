// Copyright 2023 Kistrimova Ekaterina
#ifndef TASKS_TASK_1_KISTRIMOVA_E_LETTER_COUNT_LETTER_COUNT_H_
#define TASKS_TASK_1_KISTRIMOVA_E_LETTER_COUNT_LETTER_COUNT_H_
#include <mpi.h>
#include <string>

int count_letters_seq(const std::string& str);
int count_letters_par(const std::string& str);
std::string generateRandomRow(int size);

#endif  // TASKS_TASK_1_KISTRIMOVA_E_LETTER_COUNT_LETTER_COUNT_H_
