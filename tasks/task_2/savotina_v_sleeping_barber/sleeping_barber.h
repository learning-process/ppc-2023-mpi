// Copyright 2023 Savotina Valeria
#ifndef TASKS_TASK_2_SAVOTINA_V_SLEEPING_BARBER_SLEEPING_BARBER_H_
#define TASKS_TASK_2_SAVOTINA_V_SLEEPING_BARBER_SLEEPING_BARBER_H_

#include <mpi.h>

#include <chrono>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <thread>

#define CHECK_WAIT(varName, period) (period < MPI_Wtime() - varName)

enum Tags { T_ANY = -1, T_ENTER, T_EXIT, T_LEAVE, T_WORK, T_READY };
enum Ranks { R_ANY = -2, R_ROOM = 0, R_BARBER = 1 };

void client(int rank, int clients, int seats, MPI_Comm comm);
void room(int rank, int clients, int seats, MPI_Comm comm);
void barber(int rank, int clients, int seats, MPI_Comm comm);

#endif  // TASKS_TASK_2_SAVOTINA_V_SLEEPING_BARBER_SLEEPING_BARBER_H_
