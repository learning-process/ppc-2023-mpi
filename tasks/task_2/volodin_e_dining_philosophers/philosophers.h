// Copyright 2023 Volodin Evgeniy
#ifndef TASKS_TASK_2_VOLODIN_E_DINING_PHILOSOPHERS_PHILOSOPHERS_H_
#define TASKS_TASK_2_VOLODIN_E_DINING_PHILOSOPHERS_PHILOSOPHERS_H_

#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <cstdio>
#include <random>
#include <vector>
#include <string>

int think();
int finishEating();
int eat(int* spaghetti, const std::string& ops);
void dinner(int* global_spaghetti, const std::string& ops);
int eat_alone(int proc_rank, int spaghetti, const std::string& ops);

#endif  // TASKS_TASK_2_VOLODIN_E_DINING_PHILOSOPHERS_PHILOSOPHERS_H_
