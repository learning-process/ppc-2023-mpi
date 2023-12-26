// Copyright 2023 Afanasyev Aleksey
#include "task_2/afanasyev_a_diling_philosophers_problem/philosophers.h"

Philosophers::Philosophers(int philosopher_id, int total_philosophers)
    : philosopher_id(philosopher_id), total_philosophers(total_philosophers) {}

void Philosophers::initialize() {
    left_neighbor = (philosopher_id + total_philosophers - 1) % total_philosophers;
    right_neighbor = (philosopher_id + 1) % total_philosophers;
    state = THINKING;
}

int Philosophers::getPhilosopherID() const {
    return philosopher_id;
}

int Philosophers::getTotalPhilosophers() const {
    return total_philosophers;
}

int Philosophers::getLeftNeighbor() const {
    return left_neighbor;
}

int Philosophers::getRightNeighbor() const {
    return right_neighbor;
}

int Philosophers::getState() const {
    return state;
}

void Philosophers::changeState(int new_state) {
    state = new_state;
}

void Philosophers::synchronize() {
    MPI_Barrier(MPI_COMM_WORLD);
}

void Philosophers::requestForks() {
    MPI_Request req[2];
    MPI_Irecv(nullptr, 0, MPI_INT, left_neighbor, 0, MPI_COMM_WORLD, &req[0]);
    MPI_Irecv(nullptr, 0, MPI_INT, right_neighbor, 0, MPI_COMM_WORLD, &req[1]);
    MPI_Send(nullptr, 0, MPI_INT, left_neighbor, 0, MPI_COMM_WORLD);
    MPI_Send(nullptr, 0, MPI_INT, right_neighbor, 0, MPI_COMM_WORLD);
    MPI_Waitall(2, req, MPI_STATUSES_IGNORE);
}

void Philosophers::eat() {
    changeState(EATING);
    synchronize();
}

void Philosophers::releaseForks() {
    MPI_Send(nullptr, 0, MPI_INT, left_neighbor, 0, MPI_COMM_WORLD);
    MPI_Send(nullptr, 0, MPI_INT, right_neighbor, 0, MPI_COMM_WORLD);
}

void Philosophers::think() {
    changeState(THINKING);
    synchronize();
}

void Philosophers::hungry() {
    changeState(HUNGRY);
    synchronize();
    requestForks();
    synchronize();
    eat();
    releaseForks();
    synchronize();
}
