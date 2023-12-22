// Copyright 2023 Akopyan Zal
#include "../tasks/task_2/akopyan_z_sleeping_hairdresser/sleeping_hairdresser.h"
#include <mpi.h>
#include <random>
#include <ctime>
#include <cstdlib>
#include <chrono> // NOLINT [build/c++11]
#include <thread> // NOLINT [build/c++11]
#include <sstream>
#include <iostream>
void wait() {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::this_thread::sleep_for(std::chrono::milliseconds(10 + gen() % 10));
}
void cut_hair() {
    wait();
}
void customer(int myrank) {
    int in_buffer[1] = { -1 };
    int out_buffer[1] = { myrank };
    MPI_Status status;
    srand(time(NULL) + myrank);
    while (in_buffer[0] < 0) {
       wait();
        MPI_Send(out_buffer, 1, MPI_INT, 1, CHAIR_REQUEST, MPI_COMM_WORLD);
        MPI_Recv(in_buffer, 1, MPI_INT, 1, CHAIR_RESPONSE,
          MPI_COMM_WORLD, &status);
        if (in_buffer[0] >= 0) {
            MPI_Send(out_buffer, 1, MPI_INT, 1,
                HAIR_CUT_REQUEST, MPI_COMM_WORLD);
        }
    }

    return;
}
void line(int chairs_count, int runs) {
    int in_buffer[1];
    int out_buffer[1];
    int un_free_chairs = 0;
    int* chairs = new int[chairs_count];
    MPI_Status status;
    for (int i = 0; i < runs; i++) {
        MPI_Recv(in_buffer, 1, MPI_INT, MPI_ANY_SOURCE,
            CHAIR_REQUEST, MPI_COMM_WORLD, &status);
        if (un_free_chairs < chairs_count) {
            out_buffer[0] = un_free_chairs;
            MPI_Send(out_buffer, 1, MPI_INT, in_buffer[0],
                CHAIR_RESPONSE, MPI_COMM_WORLD);

            un_free_chairs++;
            MPI_Recv(chairs + un_free_chairs, 1, MPI_INT, in_buffer[0],
                HAIR_CUT_REQUEST, MPI_COMM_WORLD, &status);
        } else {
            out_buffer[0] = -1;
            MPI_Send(out_buffer, 1, MPI_INT, in_buffer[0],
                CHAIR_RESPONSE, MPI_COMM_WORLD);
        }
        if (un_free_chairs > 0) {
            MPI_Send(&un_free_chairs, 1, MPI_INT, 0, HAIR_CUT, MPI_COMM_WORLD);
            MPI_Send(chairs + un_free_chairs, un_free_chairs,
                MPI_INT, 0, HAIR_CUT, MPI_COMM_WORLD);
            un_free_chairs = 0;
        }
    }
    return;
}

void barber(int myrank, int chairs_count, int ccount) {
    int* clients = new int[chairs_count];
    int in_buffer[1];
    MPI_Status status;
  for (int i = 0; i < ccount; i++) {
        MPI_Recv(in_buffer, 1, MPI_INT, 1,
            HAIR_CUT, MPI_COMM_WORLD, &status);
        MPI_Recv(clients, in_buffer[0], MPI_INT, 1,
            HAIR_CUT, MPI_COMM_WORLD, &status);
        for (int i = 0; i < in_buffer[0]; i++) {
            cut_hair();
        }
    }
  return;
}
