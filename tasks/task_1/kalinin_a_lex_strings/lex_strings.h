// Copyright 2023 Kalinin Alexandr
#ifndef TASKS_TASK_1_kalinin_a__lex_strings_lexicographic_strings_H_
#define TASKS_TASK_1_kalinin_a__lex_strings_lexicographic_strings_H_
#include <mpi.h>

#include <string>
#include <vector>

bool checkOrder(const std::string& str1, const std::string& str2);
bool parallelLexicographicStrings(const std::string& str1, const std::string& str2);

#endif  // TASKS_TASK_1_kalinin_a__lex_strings_lexicographic_strings_H_
