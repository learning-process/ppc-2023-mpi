// Copyright 2023 Afanasyev Aleksey
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
    Philosophers(int philosopher_id, int total_philosophers);

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
    int philosopher_id;
    int total_philosophers;
    int left_neighbor;
    int right_neighbor;

    void changeState(int new_state);
    void synchronize();
    void requestForks();
    void eat();
    void releaseForks();
};
