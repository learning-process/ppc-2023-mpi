// Copyright 2023 Savotina Valeria
#include "task_2/savotina_v_sleeping_barber/sleeping_barber.h"

void client(int rank, int clients, int seats, MPI_Comm comm) {
  if (rank < 2) {
    throw std::logic_error{"incorrect rank"};
  } else if (clients < 1) {
    throw std::logic_error{"incorrect number of clients"};
  } else if (seats < 1) {
    throw std::logic_error{"incorrect number of seats"};
  }

  int sendMes = rank;
  int recvMes = 0;
  MPI_Status status;

  // std::this_thread::sleep_for(std::chrono::milliseconds(125 * rank));

  MPI_Send(&sendMes, 1, MPI_INT, R_ROOM, T_ENTER, comm);
  MPI_Recv(&recvMes, 1, MPI_INT, R_ANY, T_ANY, comm, &status);

  if (status.MPI_TAG == T_LEAVE) {
    // std::cout << "Client leave " << rank << std::endl;
  }
}

void room(int rank, int clients, int seats, MPI_Comm comm) {
  if (rank != R_ROOM) {
    throw std::logic_error{"incorrect rank"};
  } else if (clients < 1) {
    throw std::logic_error{"incorrect number of clients"};
  } else if (seats < 1) {
    throw std::logic_error{"incorrect number of seats"};
  }

  int sendMes = 0;
  int recvMes = 0;
  int visits = 0;
  double wait = 0.0;

  MPI_Status status;
  std::queue<int> queueClients;

  do {
    // std::this_thread::sleep_for(std::chrono::milliseconds(10));

    if (visits++ < clients) {
      MPI_Recv(&recvMes, 1, MPI_INT, R_ANY, T_ENTER, comm, &status);

      if (queueClients.size() < seats) {
        queueClients.push(recvMes);
        // std::cout << "Client waiting... " << recvMes << std::endl;
      } else {
        MPI_Send(&sendMes, 1, MPI_INT, recvMes, T_LEAVE, comm);
        MPI_Send(&recvMes, 1, MPI_INT, R_BARBER, T_LEAVE, comm);
      }
    }
    if (CHECK_WAIT(wait, 0.1) && !queueClients.empty()) {
      sendMes = queueClients.front();
      queueClients.pop();

      MPI_Send(&sendMes, 1, MPI_INT, R_BARBER, T_WORK, comm);
      MPI_Recv(&recvMes, 1, MPI_INT, R_BARBER, T_READY, comm, &status);

      wait = MPI_Wtime();
    }
  } while (visits < clients || !queueClients.empty());
}

void barber(int rank, int clients, int seats, MPI_Comm comm) {
  if (rank != R_BARBER) {
    throw std::logic_error{"incorrect rank"};
  } else if (clients < 1) {
    throw std::logic_error{"incorrect number of clients"};
  } else if (seats < 1) {
    throw std::logic_error{"incorrect number of seats"};
  }

  int sendMes = 1;
  int recvMes = 0;
  MPI_Status status;

  for (int visits = 0; visits < clients; ++visits) {
    MPI_Recv(&recvMes, 1, MPI_INT, R_ROOM, MPI_ANY_TAG, comm, &status);

    if (status.MPI_TAG == T_WORK) {
      // std::cout << "Barber start working: " << recvMes << std::endl;
      MPI_Ssend(&sendMes, 1, MPI_INT, R_ROOM, T_READY, comm);

      // std::this_thread::sleep_for(std::chrono::milliseconds(750));

      // std::cout << "Barber end working: " << recvMes << std::endl;
      MPI_Send(&sendMes, 1, MPI_INT, recvMes, T_EXIT, comm);
    }
  }
  // std::cout << "Barber has THE END of the working day ^^" << std::endl;
}
