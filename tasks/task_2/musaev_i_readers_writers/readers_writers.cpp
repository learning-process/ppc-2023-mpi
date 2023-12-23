// Copyright 2023 Musaev Ilgar
#include <mpi.h>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <random>
#include "task_2/musaev_i_readers_writers/readers_writers.h"

int startTask(int writersCount) {
  const int writeRequest = 3;
  const int readRequest = 1;
  const int finishRead = 2;
  const int block = 4;
  const int done = 5;
  const int magic = 6;
  int rank, size;
  int request = 6;
  int onResponse = 0;

  MPI_Status mpiStatus;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Barrier(MPI_COMM_WORLD);
  if (writersCount < size - 2 && writersCount > 0) {
    if (rank == 0) {
      int accsess = 0;
      int readersCount = 0;
      int localMagic = 0;
      int data = 0;
      ++data;
      while (localMagic != (size - 1)) {
        MPI_Recv(&request, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &mpiStatus);
        if (request == writeRequest) {
          if (accsess == 0) {
            accsess = 1;
            data = mpiStatus.MPI_SOURCE;
            MPI_Send(&done, 1, MPI_INT, mpiStatus.MPI_SOURCE, 0, MPI_COMM_WORLD);
            accsess = 0;
          } else {
            MPI_Send(&block, 1, MPI_INT, mpiStatus.MPI_SOURCE, 0, MPI_COMM_WORLD);
          }
        } else if (request == readRequest) {
          if (accsess == 1) {
            MPI_Send(&block, 1, MPI_INT, mpiStatus.MPI_SOURCE, 0, MPI_COMM_WORLD);
          } else if (accsess == 0) {
            accsess = 2;
            ++readersCount;
            MPI_Send(&done, 1, MPI_INT, mpiStatus.MPI_SOURCE, 0, MPI_COMM_WORLD);
          } else if (accsess == 2) {
            ++readersCount;
            MPI_Send(&done, 1, MPI_INT, mpiStatus.MPI_SOURCE, 0, MPI_COMM_WORLD);
          }
        } else if (request == finishRead) {
          --readersCount;
          if (readersCount == 0)
            accsess = 0;
          MPI_Send(&done, 1, MPI_INT, mpiStatus.MPI_SOURCE, 0, MPI_COMM_WORLD);
        } else if (request == magic) {
          ++localMagic;
        }
      }
    }
    if (rank > 0 && rank <= writersCount) {
      int procMagic = 0;
      while (procMagic == 0) {
        MPI_Send(&writeRequest, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&onResponse, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &mpiStatus);
        if (onResponse == done) {
          procMagic = 1;
          MPI_Send(&magic, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
      }
    }
    if (rank > writersCount&& rank < size) {
      int procMagic = 0;
      while (procMagic == 0) {
        MPI_Send(&readRequest, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&onResponse, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &mpiStatus);
        if (onResponse == done) {
          MPI_Send(&finishRead, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
          MPI_Recv(&onResponse, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &mpiStatus);
          if (onResponse == done) {
            procMagic = 1;
            MPI_Send(&magic, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
          }
        }
      }
    }
  } else {
    throw - 1;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  return 0;
}
