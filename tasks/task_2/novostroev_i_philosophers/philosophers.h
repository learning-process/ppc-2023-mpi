// Copyright 2023 Novostroev Ivan
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <list>
#include <iostream>

#define THINKING 1
#define HUNGRY 2
#define EATING 3

class Philosophers {
 public:
    Philosophers(int philosopherId, int philosophersTotal);

    void initialize();

    int getPhilosopherID() const;
    int getTotalPhilosophers() const;
    int getLeftNeighbor() const;
    int getRightNeighbor() const;
    int getState() const;

    void think();
    void hungry();

    int state;

 private:
    int philosopherId;
    int philosophersTotal;
    int lNeighbor;
    int rNeighbor;

    void changeState(int newState);
    void synchronize();
    void requestForks();
    void releaseForks();
    void eat();
};
