// Copyright 2023 Yurin Andrey
#include <algorithm>
#include <iostream>
#include "task_2/yurin_a_readers_writers/readers_writers.h"


#define DEFAULT_TAG 0
#define READER_START_TAG 1
#define WRITER_START_TAG 2
#define READER_READY_TAG 3
#define READER_END_TAG 4
#define WRITER_READY_TAG 5
#define WRITER_END_TAG 6


void ExecuteReaders(const int& dest, size_t* processesLeft) {
    int in = 0;
    int out = 1;
    MPI_Send(&out, 1, MPI_INT, dest, READER_START_TAG, MPI_COMM_WORLD);
    MPI_Recv(&in, 1, MPI_INT, dest, READER_END_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    (*processesLeft)--;
}

void ExecuteWriter(const int& dest, size_t* processesLeft) {
    int in = 0;
    int out = 1;
    MPI_Send(&out, 1, MPI_INT, dest, WRITER_START_TAG, MPI_COMM_WORLD);
    MPI_Recv(&in, 1, MPI_INT, dest, WRITER_END_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    (*processesLeft)--;
}

void Controller(const size_t& readerCount, const size_t& writerCount) {
    int in = 0;
    int out = 1;

    int sizeWorld = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);

    size_t processesLeft = std::min(readerCount + writerCount, static_cast<size_t>(sizeWorld - 1));
    while (processesLeft) {
        MPI_Status status;
        int flag = 0;

        MPI_Iprobe(MPI_ANY_SOURCE, WRITER_READY_TAG, MPI_COMM_WORLD, &flag, MPI_STATUS_IGNORE);
        if (flag) {
            MPI_Recv(&in, 1, MPI_INT, MPI_ANY_SOURCE, WRITER_READY_TAG, MPI_COMM_WORLD, &status);
        } else {
            MPI_Recv(&in, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }

        if (status.MPI_TAG == READER_READY_TAG) {
            ExecuteReaders(status.MPI_SOURCE, &processesLeft);
        } else if (status.MPI_TAG == WRITER_READY_TAG) {
            ExecuteWriter(status.MPI_SOURCE, &processesLeft);
        }
    }
}

void Reader() {
    int in = 0;
    int out = 1;
    MPI_Send(&out, 1, MPI_INT, 0, READER_READY_TAG, MPI_COMM_WORLD);
    MPI_Recv(&in, 1, MPI_INT, 0, READER_START_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // critical section
    MPI_Send(&out, 1, MPI_INT, 0, READER_END_TAG, MPI_COMM_WORLD);
}

void Writer() {
    int in = 0;
    int out = 1;
    MPI_Send(&out, 1, MPI_INT, 0, WRITER_READY_TAG, MPI_COMM_WORLD);
    MPI_Recv(&in, 1, MPI_INT, 0, WRITER_START_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // critical section
    MPI_Send(&out, 1, MPI_INT, 0, WRITER_END_TAG, MPI_COMM_WORLD);
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
