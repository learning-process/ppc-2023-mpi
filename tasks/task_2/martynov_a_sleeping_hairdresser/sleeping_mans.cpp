// Copyright 2023 Martynov Aleksandr
#include "../tasks/task_2/martynov_a_sleeping_hairdresser/sleeping_mans.h"
#include <mpi.h>
#include <random>
#include <ctime>
#include <cstdlib>
#include <chrono> // NOLINT [build/c++11]
#include <thread> // NOLINT [build/c++11]
#include <sstream>
#include <iostream>
void sleep() {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::this_thread::sleep_for(std::chrono::milliseconds(10 + gen() % 10));
}

void bearded_man(int myrank, int _seats, int client_count) {
  MPI_Status status;
  int count = client_count;
  int* array_of_clnts = new int[_seats];
  int in_space;
  for (int i = 0; i < count; i++) {
    MPI_Recv(&in_space, 1, MPI_INT, 1, HAIR_CUT, MPI_COMM_WORLD, &status);
    MPI_Recv(array_of_clnts, in_space, MPI_INT,
      1, HAIR_CUT, MPI_COMM_WORLD, &status);
    for (int i = 0; i < in_space; i++)
      sleep();
  }
}

void visitor(int myrank) {
  std::random_device dev;
  std::mt19937 gen(dev());
  int in_space = -1;
  int on_waiting = myrank;
  MPI_Status status;
  srand(time(NULL) + myrank);
  while (in_space < 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10 + gen() % 10));
    MPI_Send(&on_waiting, 1, MPI_INT,
      1, CHAIR_REQUEST, MPI_COMM_WORLD);
    MPI_Recv(&in_space, 1, MPI_INT,
      1, CHAIR_RESPONSE, MPI_COMM_WORLD, &status);
    if (in_space >= 0) {
      MPI_Send(&on_waiting, 1, MPI_INT,
        1, HAIR_CUT_REQUEST, MPI_COMM_WORLD);
    }
  }
}

void queue(int _seats, int runs) {
  int in_space;
  int on_waiting;
  int occupied_chairs = 0;
  int count_of_seats = _seats;
  int* seats = new int[count_of_seats];
  MPI_Status status;
  for (int i = 0; i < runs; i++) {
    MPI_Recv(&in_space, 1, MPI_INT, MPI_ANY_SOURCE,
      CHAIR_REQUEST, MPI_COMM_WORLD, &status);
    if (occupied_chairs < count_of_seats) {
      on_waiting = occupied_chairs;
      MPI_Send(&on_waiting, 1, MPI_INT, in_space,
        CHAIR_RESPONSE, MPI_COMM_WORLD);
      occupied_chairs++;
      MPI_Recv(seats + occupied_chairs, 1, MPI_INT, in_space,
        HAIR_CUT_REQUEST, MPI_COMM_WORLD, &status);
    } else {
      on_waiting = -1;
      MPI_Send(&on_waiting, 1, MPI_INT, in_space,
        CHAIR_RESPONSE, MPI_COMM_WORLD);
    }
    if (occupied_chairs > 0) {
      MPI_Send(&occupied_chairs, 1, MPI_INT, 0,
        HAIR_CUT, MPI_COMM_WORLD);
      MPI_Send(seats + occupied_chairs, occupied_chairs,
        MPI_INT, 0, HAIR_CUT, MPI_COMM_WORLD);
      occupied_chairs = 0;
    }
  }
}
