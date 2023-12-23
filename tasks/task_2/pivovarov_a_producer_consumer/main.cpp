// Copyright 2023 Pivovarov Alexey

#include <gtest/gtest.h>
#include "./producer_consumer.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(ProducerConsumerTest, OneProducerOneConsumer) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    int num_producers = 1;
    int num_consumers = 1;

    std::vector<int> results_par;
    std::vector<int> results_seq;

    producer_consumer_par(num_producers, num_consumers, results_par);

    if (world.rank() == 0) {
        producer_consumer_seq(num_producers, num_consumers, results_seq);

        ASSERT_EQ(results_par, results_seq);
    }
}

TEST(ProducerConsumerTest, MultipleProducersMultipleConsumers) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    int num_producers = 2;
    int num_consumers = 2;

    std::vector<int> results_par;
    std::vector<int> results_seq;

    producer_consumer_par(num_producers, num_consumers, results_par);

    if (world.rank() == 0) {
        producer_consumer_seq(num_producers, num_consumers, results_seq);

        ASSERT_EQ(results_par, results_seq);
    }
}

TEST(ProducerConsumerTest, NoProducersNoConsumers) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    int num_producers = 0;
    int num_consumers = 0;

    std::vector<int> results_par;
    std::vector<int> results_seq;

    producer_consumer_par(num_producers, num_consumers, results_par);

    if (world.rank() == 0) {
        producer_consumer_seq(num_producers, num_consumers, results_seq);

        ASSERT_EQ(results_par, results_seq);
    }
}

TEST(ProducerConsumerTest, LargeNumberOfProducersAndConsumers) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    int num_producers = 10;
    int num_consumers = 10;

    std::vector<int> results_par;
    std::vector<int> results_seq;

    producer_consumer_par(num_producers, num_consumers, results_par);

    if (world.rank() == 0) {
        producer_consumer_seq(num_producers, num_consumers, results_seq);

        ASSERT_EQ(results_par, results_seq);
    }
}

TEST(ProducerConsumerTest, UnequalProducersAndConsumers) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    int num_producers = 2;
    int num_consumers = 5;

    std::vector<int> results_par;
    std::vector<int> results_seq;

    producer_consumer_par(num_producers, num_consumers, results_par);

    if (world.rank() == 0) {
        producer_consumer_seq(num_producers, num_consumers, results_seq);

        ASSERT_EQ(results_par, results_seq);
    }
}

int main(int argc, char **argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
