// Copyright 2023 Isaev Dmitriy
#include "task_2/isaev_d_readers_writers/readers_writers.h"

enum TAG_STATUS {
  SIMPLE_TAG = 0,
  READER_START_TAG = 1,
  WRITER_START_TAG = 2,
  READER_READY_TAG = 3,
  READER_END_TAG = 4,
  WRITER_READY_TAG = 5,
  WRITER_END_TAG = 6
};

void ExecuteWriter(const int& dest, size_t* processesLeft) {
  int inputSignal = 0;
  int outputSignal = 1;
  MPI_Send(&outputSignal, 1, MPI_INT, dest, WRITER_START_TAG, MPI_COMM_WORLD);
  MPI_Recv(&inputSignal, 1, MPI_INT, dest, WRITER_END_TAG, MPI_COMM_WORLD,
           MPI_STATUS_IGNORE);
  (*processesLeft)--;
}

void ExecuteReaders(const int& dest, size_t* processesLeft) {
  int inputSignal = 0;
  int outputSignal = 1;
  MPI_Send(&outputSignal, 1, MPI_INT, dest, READER_START_TAG, MPI_COMM_WORLD);
  MPI_Recv(&inputSignal, 1, MPI_INT, dest, READER_END_TAG, MPI_COMM_WORLD,
           MPI_STATUS_IGNORE);
  (*processesLeft)--;
}


void Controller(const size_t& readerCount, const size_t& writerCount) {
  int inputSignal = 0;
  int outputSignal = 1;

  int sizeWorld = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);

  size_t processesLeft =
      std::min(readerCount + writerCount, static_cast<size_t>(sizeWorld - 1));
  while (processesLeft) {
    MPI_Status status;
    int flag = 0;

    MPI_Iprobe(MPI_ANY_SOURCE, WRITER_READY_TAG, MPI_COMM_WORLD, &flag,
               MPI_STATUS_IGNORE);
    if (flag) {
      MPI_Recv(&inputSignal, 1, MPI_INT, MPI_ANY_SOURCE, WRITER_READY_TAG,
               MPI_COMM_WORLD, &status);
    } else {
      MPI_Recv(&inputSignal, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
               MPI_COMM_WORLD,
               &status);
    }

    if (status.MPI_TAG == READER_READY_TAG) {
      ExecuteReaders(status.MPI_SOURCE, &processesLeft);
    } else if (status.MPI_TAG == WRITER_READY_TAG) {
      ExecuteWriter(status.MPI_SOURCE, &processesLeft);
    }
  }
}

void Reader() {
  int inputSignal = 0;
  int outputSignal = 1;
  MPI_Send(&outputSignal, 1, MPI_INT, 0, READER_READY_TAG, MPI_COMM_WORLD);
  MPI_Recv(&inputSignal, 1, MPI_INT, 0, READER_START_TAG, MPI_COMM_WORLD,
           MPI_STATUS_IGNORE);

  MPI_Send(&outputSignal, 1, MPI_INT, 0, READER_END_TAG, MPI_COMM_WORLD);
}

void Writer() {
  int inputSignal = 0;
  int outputSignal = 1;
  MPI_Send(&outputSignal, 1, MPI_INT, 0, WRITER_READY_TAG, MPI_COMM_WORLD);
  MPI_Recv(&inputSignal, 1, MPI_INT, 0, WRITER_START_TAG, MPI_COMM_WORLD,
           MPI_STATUS_IGNORE);

  MPI_Send(&outputSignal, 1, MPI_INT, 0, WRITER_END_TAG, MPI_COMM_WORLD);
}

void ReadersWriters(const size_t readerCount, const size_t writerCount) {
  int rank = 0;
  int sizeWorld = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);

  if (rank == 0) {
    Controller(readerCount, writerCount);
  } else if (rank <= readerCount) {
    Reader();
  } else if (rank <= readerCount + writerCount) {
    Writer();
  }
}
