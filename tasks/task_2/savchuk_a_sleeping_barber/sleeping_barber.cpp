// Copyright 2023 Savchuk Anton
#include "tasks/task_2/savchuk_a_sleeping_barber/sleeping_barber.h"

enum MessageType{REQUEST, RESPONSE, SIGNAL, END_SIGNAL};
void Cutting_Hair(int client) {
    std::random_device dev;
    std::mt19937 rand_r(dev());
    int count = rand_r() % 5;
}
struct buffer {
    int id;
    int ProcRank;
};
void Sleeping(int* mutex, bool* thread_running) {
    std::random_device dev;
    std::mt19937 rand_r(dev());
    while (*thread_running) {
        if (!(*mutex)) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }
}
bool AnyWorks(bool* procesess, int ProcSize) {
    for (int i = 1; i < ProcSize; i++) {
        if (procesess[i])return true;
    }
    return false;
}
void DoBarberLoop(int n, int ProcSize, int ProcRank) {
    std::random_device dev;
    std::mt19937 rand_r(dev());
    buffer message;
    MPI_Status status;
    int signal = 1;
    bool working = true;
    bool free_barber = true;
    int mutex = 0, finished_clients = 0, thrown_clients = 0;
    bool thread_running = true;
    std::queue<buffer>ocered;
    bool working_procesess[10000];
    for (int i = 1; i < ProcSize; i++) {
        working_procesess[i] = true;
    }
    std::thread thr(Sleeping, &mutex, &thread_running);
    while (working) {
        if (free_barber) {
            if (!ocered.empty()) {
                signal = 1;
                MPI_Send(&signal, 1, MPI_INT, ocered.front().ProcRank,
                RESPONSE, MPI_COMM_WORLD);
                ocered.pop();
                free_barber = false;
            } else {
                mutex = 0;
            }
        }
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        switch (status.MPI_TAG) {
        case REQUEST: {
            MPI_Recv(&message, 1, MPI_2INT, MPI_ANY_SOURCE,
            REQUEST, MPI_COMM_WORLD, &status);
            if (ocered.size() < n) {
                mutex = 1;
                ocered.push(message);
            } else {
                thrown_clients++;
                signal = 0;
                MPI_Send(&signal, 1, MPI_INT, message.ProcRank,
                RESPONSE, MPI_COMM_WORLD);
            }
            break;
        }
        case SIGNAL:
        {
            MPI_Recv(&signal, 1, MPI_INT, MPI_ANY_SOURCE,
            SIGNAL, MPI_COMM_WORLD, &status);
            finished_clients++;
            free_barber = true;
            break;
        }
        case END_SIGNAL:
        {
            MPI_Recv(&signal, 1, MPI_INT, MPI_ANY_SOURCE,
            END_SIGNAL, MPI_COMM_WORLD, &status);
            working_procesess[status.MPI_SOURCE] = false;
            if (!AnyWorks(working_procesess, ProcSize))working = false;
        }
        }
    }
     thread_running = false;
     thr.join();
     MPI_Barrier(MPI_COMM_WORLD);
}
void ClientLoop(int ProcRank) {
    std::random_device dev;
    std::mt19937 rand_r(dev());
    buffer message;
    int n = rand_r() % 5;
    int signal = 1;
    MPI_Status status;
    while (n--) {
        message.id = rand_r() % 1000000;
        message.ProcRank = ProcRank;
        MPI_Send(&message, 1, MPI_2INT, 0, REQUEST, MPI_COMM_WORLD);
        MPI_Recv(&signal, 1, MPI_INT, 0, RESPONSE, MPI_COMM_WORLD, &status);
        Cutting_Hair(message.id);
        MPI_Send(&signal, 1, MPI_INT, 0, SIGNAL, MPI_COMM_WORLD);
    }
    MPI_Send(&signal, 1, MPI_INT, 0, END_SIGNAL, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
}
