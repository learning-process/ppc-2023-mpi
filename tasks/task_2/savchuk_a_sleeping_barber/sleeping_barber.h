// Copyright 2023 Savchuk Anton
#ifndef TASKS_TASK_2_SAVCHUK_A_SLEEPING_BARBER_SLEEPING_BARBER_H_
#define TASKS_TASK_2_SAVCHUK_A_SLEEPING_BARBER_SLEEPING_BARBER_H_

#include <mpi.h>

#include <iostream>
#include <queue>
#include <stdexcept>

#define CHECK_WAIT(varName, period) (period < MPI_Wtime() - varName)

enum Tags { T_ANY = -1, T_ENTER, T_EXIT, T_LEAVE, T_WORK, T_READY };
enum Ranks { R_ANY = -2, R_ROOM = 0, R_BARBER = 1 };

void CustomClient(int rank, int clients, int seats, MPI_Comm comm);
void CustomRoom(int rank, int clients, int seats, MPI_Comm comm);
void CustomBarber(int rank, int clients, int seats, MPI_Comm comm);

#endif  // TASKS_TASK_2_SAVCHUK_A_SLEEPING_BARBER_SLEEPING_BARBER_H_
