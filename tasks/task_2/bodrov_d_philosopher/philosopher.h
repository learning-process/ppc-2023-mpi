// Copyright 2023 Bodrov Daniil
#ifndef TASKS_TASK_2_BODROV_D_PHILOSOPHER_PHILOSOPHER_H_
#define TASKS_TASK_2_BODROV_D_PHILOSOPHER_PHILOSOPHER_H_

#include <mpi.h>

class Philosopher {
 public:
    Philosopher(int philosopher_id, int num_philosophers);
    void runSimulation();
    void grabForks(int left_fork, int right_fork);
    void releaseForks(int left_fork, int right_fork);
    void think();
    void eat();
 private:
    int phId;
    int numPh;
    MPI_Status status;
};

#endif  // TASKS_TASK_2_BODROV_D_PHILOSOPHER_PHILOSOPHER_H_
