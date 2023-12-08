// Copyright 2023 Bodrov Daniil
#include <gtest/gtest.h>
#include "./philosopher.h"

TEST(PhilosopherTest, GrabAndReleaseForks) {
    int rank, numProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    int numPhilosophers = numProcesses;
    Philosopher philosopher(rank, numPhilosophers);

    if (numPhilosophers > 1) {
        int left_fork = (rank + numPhilosophers - 1) % numPhilosophers;
        int right_fork = (rank + 1) % numPhilosophers;
        philosopher.grabForks(left_fork, right_fork);
        philosopher.releaseForks(left_fork, right_fork);
    }
}

TEST(PhilosopherTest, ThinkAndEat) {
    int rank, numProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    int numPhilosophers = numProcesses;
    Philosopher philosopher(rank, numPhilosophers);

    philosopher.think();
    philosopher.eat();
}

TEST(PhilosopherTest, RunSimulation) {
    int rank, numProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    int numPhilosophers = numProcesses;
    Philosopher philosopher(rank, numPhilosophers);

    philosopher.runSimulation();
}

TEST(PhilosopherTest, DifferentPhilosophers) {
    int rank, numProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    int numPhilosophers = numProcesses;
    Philosopher philosopher(rank, numPhilosophers);

    if (numPhilosophers > 1) {
        int left_fork = (rank + numPhilosophers - 1) % numPhilosophers;
        int right_fork = (rank + 1) % numPhilosophers;
        philosopher.grabForks(left_fork, right_fork);
        philosopher.releaseForks(left_fork, right_fork);
    }
}

TEST(PhilosopherTest, SimulateDining) {
    int rank, numProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    int numPhilosophers = numProcesses;
    Philosopher philosopher(rank, numPhilosophers);

    philosopher.runSimulation();
}

int main(int argc, char **argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
