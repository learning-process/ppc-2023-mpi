// Copyright 2023 Filatov Maxim
#include <gtest/gtest.h>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "task_2/filatov_m_producers_consumers/producers_consumers.h"

TEST(ProducersConsumersTest, one_producers_one_consumers) {
    boost::mpi::communicator communicator;
    std::vector<int> results_par;
    std::vector<int> results_seq;
    int num_producers = 1;
    int num_consumers = 1;
    producer_consumer_par(num_producers, num_consumers, results_par);
    if (!communicator.rank()) {
        producer_consumer_seq(num_producers, num_consumers, results_seq);
        ASSERT_EQ(results_par, results_seq);
    }
}

TEST(ProducersConsumersTest, two_producers_two_consumers) {
    boost::mpi::communicator communicator;
    std::vector<int> results_par;
    std::vector<int> results_seq;
    int num_producers = 2;
    int num_consumers = 2;
    producer_consumer_par(num_producers, num_consumers, results_par);
    if (!communicator.rank()) {
        producer_consumer_seq(num_producers, num_consumers, results_seq);
        ASSERT_EQ(results_par, results_seq);
    }
}

TEST(ProducersConsumersTest, zero_producers_zero_consumers) {
    boost::mpi::communicator communicator;
    std::vector<int> results_par;
    std::vector<int> results_seq;
    int num_producers = 0;
    int num_consumers = 0;
    producer_consumer_par(num_producers, num_consumers, results_par);
    if (!communicator.rank()) {
        producer_consumer_seq(num_producers, num_consumers, results_seq);
        ASSERT_EQ(results_par, results_seq);
    }
}

TEST(ProducersConsumersTest, five_producers_five_consumers) {
    boost::mpi::communicator communicator;
    std::vector<int> results_par;
    std::vector<int> results_seq;
    int num_producers = 5;
    int num_consumers = 5;
    producer_consumer_par(num_producers, num_consumers, results_par);
    if (!communicator.rank()) {
        producer_consumer_seq(num_producers, num_consumers, results_seq);
        ASSERT_EQ(results_par, results_seq);
    }
}

TEST(ProducersConsumersTest, four_producers_five_consumers) {
    boost::mpi::communicator communicator;
    std::vector<int> results_par;
    std::vector<int> results_seq;
    int num_producers = 4;
    int num_consumers = 5;
    producer_consumer_par(num_producers, num_consumers, results_par);
    if (!communicator.rank()) {
        producer_consumer_seq(num_producers, num_consumers, results_seq);
        ASSERT_EQ(results_par, results_seq);
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
