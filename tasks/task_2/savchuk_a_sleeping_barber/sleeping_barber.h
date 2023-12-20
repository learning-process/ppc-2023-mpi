// Copyright 2023 Savchuk Anton
#ifndef TASKS_TASK_2_SAVCHUK_A_SLEEPING_BARBER_SLEEPING_BARBER_H_
#define TASKS_TASK_2_SAVCHUK_A_SLEEPING_BARBER_SLEEPING_BARBER_H_

#include<time.h>
#include<stdlib.h>
#include<mpi.h>
#include<iostream>
#include<thread>
#include<chrono>
#include<random>
#include<queue>

void DoBarberLoop(int n, int ProcSize, int ProcRank);
void Cutting_Hair(int client);
void ClientLoop(int ProcRank);
void Sleeping(int* mutex, bool* thread_running);
bool AnyWorks(bool* procesess, int ProcSize);

#endif  // TASKS_TASK_2_SAVCHUK_A_SLEEPING_BARBER_SLEEPING_BARBER_H_
