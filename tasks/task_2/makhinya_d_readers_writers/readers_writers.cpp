// Copyright 2023 Makhinya Danil
#include <vector>
#include <random>
#include <numeric>
#include <functional>
#include <random>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

#include "task_2/makhinya_d_readers_writers/readers_writers.h"

#define IS_ACCESS_SEMOPHORE 0
#define IS_RC_SEMOPHORE 1
#define IS_S_SEMOPHORE 2
#define IS_SHARED_DATA 3
#define IS_READ_COUNT 4

void run_random_order_read_and_write(int seed, uint16_t count_people) {
    std::srand(seed);

    start_task();

    for (int i = 0; i < count_people; ++i) {
        if (std::rand() & 1) {
            write_data(1);
        } else { 
            read_data();
        }
    }
}

void run_random_order_read_and_write(int seed, uint16_t count_writer, uint16_t count_reader) {
    std::srand(seed);

    start_task();

    for (int i = 0; i < count_writer + count_reader; ++i) {
        if (std::rand() & 1 || count_reader == 0) {
            write_data(1);
            count_writer--;
        } else { 
            read_data();
            count_reader--;
        }
    }
}

void P(boost::mpi::communicator& world, int tag, int& semophore) {
    do {
        world.recv(0, tag, semophore);
    } while (semophore == 0);
    
    semophore--;
    world.send(0, tag, semophore);
}

void V(boost::mpi::communicator& world, int tag, int& semophore) {
    world.recv(0, tag, semophore);
    semophore++;
    world.send(0, tag, semophore);
}


void start_task() {
    boost::mpi::communicator world;
    int sm_access = 1, sm_rc = 1, sm_s = 1;
    int read_count = 0;

    int shared_data = 0;

    double local_res = 0.0;
    if (world.rank() == 0) {
        auto sz = world.size();

        for (int proc = 1; proc < sz; proc++) {
            world.send(proc, IS_ACCESS_SEMOPHORE, sm_access);
            world.send(proc, IS_RC_SEMOPHORE, sm_rc);
            world.send(proc, IS_S_SEMOPHORE, sm_s);
            world.send(proc, IS_SHARED_DATA, shared_data);
            world.send(proc, IS_READ_COUNT, read_count);
        }
    }
}

int read_data() {
    boost::mpi::communicator world;
    int sm_access = 0, sm_rc = 0, sm_s = 0;
    int read_count = 0;

    int res = 0;

    if (world.rank() != 0) {
        P(world, IS_S_SEMOPHORE, sm_s);
        V(world, IS_S_SEMOPHORE, sm_s);

        P(world, IS_READ_COUNT, sm_rc);

        world.recv(0, IS_READ_COUNT, read_count);
        read_count++;
        world.send(0, IS_READ_COUNT, read_count);

        if(read_count == 1) {
            P(world, IS_ACCESS_SEMOPHORE, sm_access);
        }

        V(world, IS_RC_SEMOPHORE, sm_rc);

        world.recv(0, IS_SHARED_DATA, res);

        P(world, IS_RC_SEMOPHORE, sm_rc);

        world.recv(0, IS_READ_COUNT, read_count);
        read_count--;
        world.send(0, IS_READ_COUNT, read_count);

        if(read_count == 0) {
            V(world, IS_ACCESS_SEMOPHORE, sm_access);
        }

        V(world, IS_RC_SEMOPHORE, sm_rc);
    }

    return res;
}

void write_data(int new_data) {
    boost::mpi::communicator world;
    int sm_access = 0, sm_rc = 0, sm_s = 0;
    int read_count = 0;

    int shared_data = 0;

    if (world.rank() != 0) {
        P(world, IS_RC_SEMOPHORE, sm_rc);
        P(world, IS_ACCESS_SEMOPHORE, sm_access);
        V(world, IS_S_SEMOPHORE, sm_s);

        world.recv(0, IS_SHARED_DATA, shared_data);
        shared_data = new_data;
        world.send(0, IS_SHARED_DATA, shared_data);

        V(world, IS_ACCESS_SEMOPHORE, sm_access);
    }
}
