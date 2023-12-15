// Copyright 2023 Volodin Evgeniy
#include "task_2/volodin_e_dining_philosophers/philosophers.h"


enum states_of_philosophers {
    Thinking,
    Eating,
    Hungry
};

int eat_alone(int proc_rank, int portion_spaghetti, const std::string& ops) {
    if (ops ==  "+") {
        return portion_spaghetti += proc_rank;
    } else if (ops == "-") {
        return portion_spaghetti -= proc_rank;
    } else if (ops == "*") {
        return portion_spaghetti *= proc_rank;
    } else if (ops == "**") {
        return portion_spaghetti = std::pow(portion_spaghetti, proc_rank);
    } else if (ops == "/") {
        return portion_spaghetti /= proc_rank;
    }
    return -1;
}

int think() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> think_rnd(2800, 4000);
    for (int i = 0; i < think_rnd(rng); ++i) {
        continue;
    }
    return Hungry;
}

int finishEating() {
    return Thinking;
}

int eat(int* portion_spaghetti, const std::string& ops) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    if (ops ==  "+") {
        *portion_spaghetti += proc_rank;
    } else if (ops == "-") {
        *portion_spaghetti -= proc_rank;
    } else if (ops == "*") {
        *portion_spaghetti *= proc_rank;
    } else if (ops == "**") {
        *portion_spaghetti = std::pow(*portion_spaghetti, proc_rank);
    } else if (ops == "/") {
        *portion_spaghetti /= proc_rank;
    }
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> eat_rnd(1300, 2600);
    for (int i = 0; i < eat_rnd(rng); ++i) {
        continue;
    }
    return Eating;
}

void dinner(int* lunch, const std::string& ops) {
    int state;
    int *message;
    MPI_Status status;
    message = reinterpret_cast<int*>(malloc(sizeof(int)));

    int proc_rank, proc_num;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    MPI_Barrier(MPI_COMM_WORLD);

    std::vector<int> displs(proc_num);
    std::vector<int> send_counts(proc_num, 1);
    for (int i = 0; i < proc_num; ++i) {
        displs[i] = i;
    }
    int portion_spaghetti;
    MPI_Scatterv(lunch, send_counts.data(), displs.data(), MPI_INT,
    &portion_spaghetti, send_counts[proc_rank], MPI_INT, 0, MPI_COMM_WORLD);

    if (proc_rank == 0) {
        int *spoons;
        int hungry_philosophers = proc_num - 1;
        spoons = reinterpret_cast<int*>(malloc(sizeof(int) * (proc_num - 1)));
        for (int i = 0; i < (proc_num - 1); ++i) {
            spoons[i] = 1;
        }
        while (hungry_philosophers > 0) {
            MPI_Recv(message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (status.MPI_TAG == 0) {
                if ((spoons[(status.MPI_SOURCE - 1)] == 1) && (spoons[(status.MPI_SOURCE) % (proc_num - 1)] == 1)) {
                    spoons[(status.MPI_SOURCE - 1)] = 0;
                    spoons[status.MPI_SOURCE % (proc_num - 1)] = 0;
                    MPI_Send(message, 1, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
                } else {
                    MPI_Send(message, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
                }
            } else if (status.MPI_TAG == 1) {
                spoons[status.MPI_SOURCE - 1] = 1;
                spoons[(status.MPI_SOURCE) % (proc_num - 1)] = 1;
                MPI_Send(message, 1, MPI_INT, status.MPI_SOURCE, 2, MPI_COMM_WORLD);
                hungry_philosophers--;
            }
        }
        goto label1;
    }

    state = think();
    do {
        MPI_Send(message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(message, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        for (int i = 0; i < 10000; ++i) {
            continue;
        }
    } while (status.MPI_TAG == 0);
    state = eat(&portion_spaghetti, ops);
    MPI_Send(message, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    MPI_Recv(message, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
    state = finishEating();
label1:
    MPI_Gatherv(&portion_spaghetti, 1, MPI_INT, lunch,
        send_counts.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);
    return;
}
