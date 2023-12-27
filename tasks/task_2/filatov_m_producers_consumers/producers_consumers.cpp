// Copyright 2023 Filatov Maxim
#include "task_2/filatov_m_producers_consumers/producers_consumers.h"

namespace mpi = boost::mpi;

void process_buffer(
    int rank,
    int num_producers,
    int num_consumers,
    std::vector<int>* buffer,
    int* count_producer,
    int* count_consumer,
    mpi::communicator* world) {
    if (rank < num_producers) {
        for (int i = 0; i < num_producers; ++i) {
            while (buffer->size() >= num_consumers) {;}
            buffer->push_back(rank * 100 + i);
            (*count_producer)++;
            if (*count_producer == num_producers) {
                *count_producer = 0;
                (*count_consumer)++;
            }
        }
    } else {
        for (int i = 0; i < num_consumers; ++i) {
            while (buffer->empty()) {;}
            int data = buffer->back();
            buffer->pop_back();
            (*count_consumer)++;
            if (*count_consumer == num_consumers) {
                *count_consumer = 0;
                (*count_producer)++;
            }
            std::cout << "Consumer "
                << rank - num_producers
                << " consumed: "
                << data << std::endl;
        }
    }
}

void producer_consumer_seq(
    int num_producers,
    int num_consumers,
    std::vector<int> results) {
    mpi::environment env;
    mpi::communicator world;
    int rank = world.rank();
    int size = world.size();
    std::vector<int> buffer;
    int count_producer = 0;
    int count_consumer = 0;
    process_buffer(
        rank,
        num_producers,
        num_consumers,
        &buffer,
        &count_producer,
        &count_consumer,
        &world);
    results.push_back(1);
}

std::pair<int, int> calculate_work_range(int rank, int size, int total_tasks) {
    int tasks_per_process = total_tasks / size;
    int remainder = total_tasks % size;
    int start = rank * tasks_per_process + std::min(rank, remainder);
    int extra_task = (rank < remainder) ? 1 : 0;
    return { start, start + tasks_per_process + extra_task };
}

void do_producer_work(int start, int end, int rank) {
    for (int i = start; i < end; ++i) {
        int data = i;
        std::cout << "Producer " << rank << " produced: " << data << std::endl;
    }
}

void do_consumer_work(int start, int end, int rank) {
    for (int i = start; i < end; ++i) {
        int data = i;
        std::cout << "Consumer "
            << rank
            << " consumed: "
            << data
            << ", Processed: "
            << data * 2
            << std::endl;
    }
}

void producer_consumer_par(
    int num_producers,
    int num_consumers,
    std::vector<int> results) {
    mpi::environment env;
    mpi::communicator world;
    int rank = world.rank();
    int size = world.size();
    auto [prod_start, prod_end] = calculate_work_range(
        rank,
        size,
        num_producers);
    do_producer_work(prod_start, prod_end, rank);
    world.barrier();
    auto [cons_start, cons_end] = calculate_work_range(
        rank,
        size,
        num_consumers);
    do_consumer_work(cons_start, cons_end, rank);
    world.barrier();
    results.push_back(rank);
}
