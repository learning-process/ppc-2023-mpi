// Copyright 2023 Savchuk Anton
#include "task_2/savchuk_a_sleeping_barber/sleeping_barber.h"

void CustomClient(int customRank, int customClients,
                    int customSeats, MPI_Comm comm) {
    if (customRank < 2) {
        throw std::logic_error{ "incorrect customRank" };
    } else if (customClients < 1) {
        throw std::logic_error{ "incorrect number of customClients" };
    } else if (customSeats < 1) {
        throw std::logic_error{ "incorrect number of customSeats" };
    }

    int sendMes = customRank;
    int recvMes = 0;
    MPI_Status status;

    MPI_Send(&sendMes, 1, MPI_INT, R_ROOM, T_ENTER, comm);
    MPI_Recv(&recvMes, 1, MPI_INT, R_ANY, T_ANY, comm, &status);
}

void CustomRoom(int customRank, int customClients,
                 int customSeats, MPI_Comm comm) {
    if (customRank != R_ROOM) {
        throw std::logic_error{ "incorrect customRank" };
    } else if (customClients < 1) {
        throw std::logic_error{ "incorrect number of customClients" };
    } else if (customSeats < 1) {
        throw std::logic_error{ "incorrect number of customSeats" };
    }

    int sendMes = 0;
    int recvMes = 0;
    int visits = 0;
    double wait = 0.0;

    MPI_Status status;
    std::queue<int> queuecustomClients;

    do {
        if (visits++ < customClients) {
            MPI_Recv(&recvMes, 1, MPI_INT, R_ANY, T_ENTER, comm, &status);

            if (queuecustomClients.size() < customSeats) {
                queuecustomClients.push(recvMes);
            } else {
                MPI_Send(&sendMes, 1, MPI_INT, recvMes, T_LEAVE, comm);
                MPI_Send(&recvMes, 1, MPI_INT, R_BARBER, T_LEAVE, comm);
            }
        }
        if (CHECK_WAIT(wait, 0.1) && !queuecustomClients.empty()) {
            sendMes = queuecustomClients.front();
            queuecustomClients.pop();

            MPI_Send(&sendMes, 1, MPI_INT, R_BARBER, T_WORK, comm);
            MPI_Recv(&recvMes, 1, MPI_INT, R_BARBER, T_READY, comm, &status);

            wait = MPI_Wtime();
        }
    } while (visits < customClients || !queuecustomClients.empty());
}

void CustomBarber(int customRank, int customClients,
                 int customSeats, MPI_Comm comm) {
    if (customRank != R_BARBER) {
        throw std::logic_error{ "incorrect customRank" };
    } else if (customClients < 1) {
        throw std::logic_error{ "incorrect number of customClients" };
    } else if (customSeats < 1) {
        throw std::logic_error{ "incorrect number of customSeats" };
    }

    int sendMes = 1;
    int recvMes = 0;
    MPI_Status status;

    for (int visits = 0; visits < customClients; ++visits) {
        MPI_Recv(&recvMes, 1, MPI_INT, R_ROOM, MPI_ANY_TAG, comm, &status);

        if (status.MPI_TAG == T_WORK) {
            MPI_Ssend(&sendMes, 1, MPI_INT, R_ROOM, T_READY, comm);
            MPI_Send(&sendMes, 1, MPI_INT, recvMes, T_EXIT, comm);
        }
    }
}
