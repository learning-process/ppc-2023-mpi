// Copyright 2023 Mironov Ilya
#include <gtest/gtest.h>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "./prod_cons.h"

TEST(ProducersAndConsumers, ProducersHalf1) {
    boost::mpi::communicator world;
    int acting_processes = world.size() - 1;
    ProducerConsumer(acting_processes - acting_processes / 2);
}

TEST(ProducersAndConsumers, ProducersHalf2) {
    boost::mpi::communicator world;
    int acting_processes = world.size() - 1;
    ProducerConsumer(acting_processes - acting_processes / 2);
}

TEST(ProducersAndConsumers, ProducersHalf3) {
    boost::mpi::communicator world;
    int acting_processes = world.size() - 1;
    ProducerConsumer(acting_processes - acting_processes / 2);
}

TEST(ProducersAndConsumers, ProducersThird1) {
    boost::mpi::communicator world;
    int acting_processes = world.size() - 1;
    ProducerConsumer(acting_processes - acting_processes / 3);
}

TEST(ProducersAndConsumers, ProducersThird2) {
    boost::mpi::communicator world;
    int acting_processes = world.size() - 1;
    ProducerConsumer(acting_processes - acting_processes / 3);
}


int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
