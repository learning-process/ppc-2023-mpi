// Copyright 2023 Makhinya Danil
#include <vector>
#include <random>
#include <numeric>
#include <functional>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

#include "task_2/makhinya_d_readers_writers/readers_writers.h"

#define DEFAULT_TAG 0
#define READER_START_TAG 1
#define WRITER_START_TAG 2
#define READER_READY_TAG 3
#define READER_END_TAG 4
#define WRITER_READY_TAG 5
#define WRITER_END_TAG 6

void read() {
    int in = 0;
    int out = 1;
    MPI_Send(&out, 1, MPI_INT, 0, READER_READY_TAG, MPI_COMM_WORLD);
    MPI_Recv(&in, 1, MPI_INT, 0, READER_START_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // critical section
    MPI_Send(&out, 1, MPI_INT, 0, READER_END_TAG, MPI_COMM_WORLD);
}

void write() {
    int in = 0;
    int out = 1;
    MPI_Send(&out, 1, MPI_INT, 0, WRITER_READY_TAG, MPI_COMM_WORLD);
    MPI_Recv(&in, 1, MPI_INT, 0, WRITER_START_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // critical section
    MPI_Send(&out, 1, MPI_INT, 0, WRITER_END_TAG, MPI_COMM_WORLD);
}

void FinishCompletedReaders(
        uint16_t* localReaderCount,
        uint16_t* processesLeft
        ) {
    int in = 0;
    MPI_Recv(&in, 1, MPI_INT, MPI_ANY_SOURCE, READER_END_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    (*localReaderCount)--;
    (*processesLeft)--;
}

void StartNewReaders(uint16_t* localReaderCount) {
    MPI_Status status;
    int in = 0;
    int out = 1;

    MPI_Recv(&in, 1, MPI_INT, MPI_ANY_SOURCE, READER_READY_TAG, MPI_COMM_WORLD, &status);
    MPI_Send(&out, 1, MPI_INT, status.MPI_SOURCE, READER_START_TAG, MPI_COMM_WORLD);

    (*localReaderCount)++;
}

void ExecuteWriter(const int& dest, uint16_t* processesLeft) {
    int in = 0;
    int out = 1;
    MPI_Send(&out, 1, MPI_INT, dest, WRITER_START_TAG, MPI_COMM_WORLD);
    MPI_Recv(&in, 1, MPI_INT, dest, WRITER_END_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    (*processesLeft)--;
}

void handler(uint16_t readerCount, uint16_t writerCount) {
    int in = 0;
    int out = 1;

    int sizeWorld = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);

    uint16_t processesLeft = std::min((int)(readerCount + writerCount), (sizeWorld - 1));
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
            uint16_t localReaderCount = 1;
            MPI_Send(&out, 1, MPI_INT, status.MPI_SOURCE, READER_START_TAG, MPI_COMM_WORLD);

            MPI_Iprobe(MPI_ANY_SOURCE, WRITER_READY_TAG, MPI_COMM_WORLD, &flag, MPI_STATUS_IGNORE);
            while (!flag && localReaderCount != 0) {
                int readerEndFlag = 0;
                MPI_Iprobe(MPI_ANY_SOURCE, READER_END_TAG, MPI_COMM_WORLD, &readerEndFlag, MPI_STATUS_IGNORE);
                if (readerEndFlag) {
                    FinishCompletedReaders(&localReaderCount, &processesLeft);
                }

                int readerReadyFlag = 0;
                MPI_Iprobe(MPI_ANY_SOURCE, READER_READY_TAG, MPI_COMM_WORLD, &readerReadyFlag, MPI_STATUS_IGNORE);
                if (readerReadyFlag) {
                    StartNewReaders(&localReaderCount);
                }

                MPI_Iprobe(MPI_ANY_SOURCE, WRITER_READY_TAG, MPI_COMM_WORLD, &flag, MPI_STATUS_IGNORE);
            }

            while (localReaderCount) {
                FinishCompletedReaders(&localReaderCount, &processesLeft);
            }

        } else if (status.MPI_TAG == WRITER_READY_TAG) {
            ExecuteWriter(status.MPI_SOURCE, &processesLeft);
        }
    }
}

void run_problem_readers_writers(uint16_t readerCount, uint16_t writerCount) {
    int rank = 0;
    int sizeWorld = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);

    if (rank == 0) {
        handler(readerCount, writerCount);
    } else if (rank <= readerCount) {
        read();
    } else if (rank <= readerCount + writerCount) {
        write();
    }
}