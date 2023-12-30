// Copyright 2023 Novostroev Ivan
#include "task_2/novostroev_i_philosophers/philosophers.h"

Philosophers::Philosophers(int philosopherId, int philosophersTotal)
    : philosopherId(philosopherId), philosophersTotal(philosophersTotal) {}

void Philosophers::initialize() {
    lNeighbor = (philosopherId + philosophersTotal - 1) % philosophersTotal;
    rNeighbor = (philosopherId + 1) % philosophersTotal;
    state = THINKING;
}

int Philosophers::getPhilosopherID() const {
    return philosopherId;
}

int Philosophers::getTotalPhilosophers() const {
    return philosophersTotal;
}

int Philosophers::getLeftNeighbor() const {
    return lNeighbor;
}

int Philosophers::getRightNeighbor() const {
    return rNeighbor;
}

int Philosophers::getState() const {
    return state;
}

void Philosophers::changeState(int newState) {
    state = newState;
}

void Philosophers::synchronize() {
    MPI_Barrier(MPI_COMM_WORLD);
}

void Philosophers::eat() {
    changeState(EATING);
    synchronize();
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

void Philosophers::requestForks() {
    MPI_Request req[2];
    MPI_Irecv(nullptr, 0, MPI_INT, lNeighbor, 0, MPI_COMM_WORLD, &req[0]);
    MPI_Irecv(nullptr, 0, MPI_INT, rNeighbor, 0, MPI_COMM_WORLD, &req[1]);
    MPI_Send(nullptr, 0, MPI_INT, lNeighbor, 0, MPI_COMM_WORLD);
    MPI_Send(nullptr, 0, MPI_INT, rNeighbor, 0, MPI_COMM_WORLD);
    MPI_Waitall(2, req, MPI_STATUSES_IGNORE);
}

void Philosophers::releaseForks() {
    MPI_Send(nullptr, 0, MPI_INT, lNeighbor, 0, MPI_COMM_WORLD);
    MPI_Send(nullptr, 0, MPI_INT, rNeighbor, 0, MPI_COMM_WORLD);
}
