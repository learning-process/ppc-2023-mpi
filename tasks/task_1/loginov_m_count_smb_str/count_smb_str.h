//  Copyright 2023 Loginov Maxim
#ifndef TASKS_TASK_1_LOGINOV_M_COUNT_SMB_STR_COUNT_SMB_STR_H_
#define TASKS_TASK_1_LOGINOV_M_COUNT_SMB_STR_COUNT_SMB_STR_H_

#include <mpi.h>
#include <vector>
#include <string>

int calcFreqSeq(const std::string& str, char target);
int calcFreqPar(const std::string& str, char target);


#endif  // TASKS_TASK_1_LOGINOV_M_COUNT_SMB_STR_COUNT_SMB_STR_H_
