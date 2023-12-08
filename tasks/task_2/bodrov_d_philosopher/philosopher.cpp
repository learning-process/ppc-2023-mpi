// Copyright 2023 Bodrov Daniil
#include "task_2/bodrov_d_philosopher/philosopher.h"
#include <iostream>
#include <chrono>
#include <thread>

Philosopher::Philosopher(int philosopher_id, int num_philosophers) : philosopherId(philosopher_id), numPhilosophers(num_philosophers) {
}

void Philosopher::grabForks(int left_fork, int right_fork) {
    int send_msg = philosopherId;
    MPI_Send(&send_msg, 1, MPI_INT, left_fork, 0, MPI_COMM_WORLD);
    MPI_Send(&send_msg, 1, MPI_INT, right_fork, 0, MPI_COMM_WORLD);

    MPI_Recv(&send_msg, 1, MPI_INT, left_fork, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&send_msg, 1, MPI_INT, right_fork, 0, MPI_COMM_WORLD, &status);
}

void Philosopher::releaseForks(int left_fork, int right_fork) {
    int send_msg = philosopherId;
    MPI_Send(&send_msg, 1, MPI_INT, left_fork, 0, MPI_COMM_WORLD);
    MPI_Send(&send_msg, 1, MPI_INT, right_fork, 0, MPI_COMM_WORLD);
}

void Philosopher::think() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Philosopher::eat() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Philosopher::runSimulation() {
    for (int i = 0; i < 5; ++i) {
        think();
        int left_fork = (philosopherId + numPhilosophers - 1) % numPhilosophers;
        int right_fork = (philosopherId + 1) % numPhilosophers;
        grabForks(left_fork, right_fork);
        eat();
        releaseForks(left_fork, right_fork);
    }
}
