// Copyright 2023 Safarov Nurlan
#ifndef TASKS_TASK_1_SAFAROV_N_NON_MATCHING_SYMBOLS_NUMBER_OF_DIFFERENT_SYMBOLS_H_
#define TASKS_TASK_1_SAFAROV_N_NON_MATCHING_SYMBOLS_NUMBER_OF_DIFFERENT_SYMBOLS_H_

#include <mpi.h>

#include <random>
#include <string>
#include <algorithm>
#include <boost/chrono.hpp>

int numberOfNonMatchingCharacters_Consistently(const std::string& firstString,
    const std::string& secondString);
int numberOfNonMatchingCharacters_Parallel(std::string* firstString,
    std::string* secondString);
std::string generateRandomString(const int& length);


#endif  // TASKS_TASK_1_SAFAROV_N_NON_MATCHING_SYMBOLS_NUMBER_OF_DIFFERENT_SYMBOLS_H_
