// Copyright 2023 Pivovarov Alexey

#include "task_2/pivovarov_a_producer_consumer/producer_consumer.h"

void producer_consumer_seq(int num_producers, int num_consumers, std::vector<int> results) {
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> buffer;
    int count_producer = 0;
    int count_consumer = 0;

    auto producer_process = [&](int id) {
        for (int i = 0; i < num_producers; ++i) {
            while (buffer.size() >= num_consumers) {
                MPI_Barrier(MPI_COMM_WORLD);
            }

            buffer.push_back(id * 100 + i);

            count_producer++;
            if (count_producer == num_producers) {
                count_producer = 0;
                count_consumer++;
            }
        }
    };

    auto consumer_process = [&](int id) {
        for (int i = 0; i < num_consumers; ++i) {
            while (buffer.empty()) {
                MPI_Barrier(MPI_COMM_WORLD);
            }

            int data = buffer.back();
            buffer.pop_back();

            count_consumer++;
            if (count_consumer == num_consumers) {
                count_consumer = 0;
                count_producer++;
            }
            std::cout << "Consumer " << id << " consumed: " << data << std::endl;
            MPI_Barrier(MPI_COMM_WORLD);
        }
    };

    if (rank < num_producers) {
        producer_process(rank);
    } else {
        consumer_process(rank - num_producers);
    }

    results.push_back(1);
}


void producer_consumer_par(int num_producers, int num_consumers, std::vector<int> results) {
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int tasks_per_process = num_producers / size;
    int remainder = num_producers % size;

    int start = rank * tasks_per_process + std::min(rank, remainder);
    int end = start + tasks_per_process + (rank < remainder ? 1 : 0);

    for (int i = start; i < end; ++i) {
        int data = i;
        std::cout << "Producer " << rank << " produced: " << data << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    tasks_per_process = num_consumers / size;
    remainder = num_consumers % size;

    start = rank * tasks_per_process + std::min(rank, remainder);
    end = start + tasks_per_process + (rank < remainder ? 1 : 0);

    for (int i = start; i < end; ++i) {
        int data = i;
        std::cout << "Consumer " << rank << " consumed: " << data << ", Processed: " << data * 2 << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    results.push_back(1);
}
