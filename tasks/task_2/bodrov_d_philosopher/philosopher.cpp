// Copyright 2023 Bodrov Daniil
#include "task_2/bodrov_d_philosopher/philosopher.h"
#include "unapproved/unapproved.h"

Philosopher::Philosopher(int ph_id, int num_ph) : phId(ph_id), numPh(num_ph) {
}

void Philosopher::grabForks(int left_fork, int right_fork) {
    int send_msg = phId;
    MPI_Send(&send_msg, 1, MPI_INT, left_fork, 0, MPI_COMM_WORLD);
    MPI_Send(&send_msg, 1, MPI_INT, right_fork, 0, MPI_COMM_WORLD);

    MPI_Recv(&send_msg, 1, MPI_INT, left_fork, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&send_msg, 1, MPI_INT, right_fork, 0, MPI_COMM_WORLD, &status);
}

void Philosopher::releaseForks(int left_fork, int right_fork) {
    int send_msg = phId;
    MPI_Send(&send_msg, 1, MPI_INT, left_fork, 0, MPI_COMM_WORLD);
    MPI_Send(&send_msg, 1, MPI_INT, right_fork, 0, MPI_COMM_WORLD);
}

void Philosopher::think() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void Philosopher::eat() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void Philosopher::runSimulation() {
    think();
    int left_fork = (phId + numPh - 1) % numPh;
    int right_fork = (phId + 1) % numPh;
    grabForks(left_fork, right_fork);
    eat();
    releaseForks(left_fork, right_fork);
}
