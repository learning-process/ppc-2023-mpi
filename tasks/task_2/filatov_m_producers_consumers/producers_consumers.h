// Copyright 2023 Filatov Maxim
#ifndef TASKS_TASK_2_FILATOV_M_PRODUCERS_CONSUMERS_PRODUCERS_CONSUMERS_H_
#define TASKS_TASK_2_FILATOV_M_PRODUCERS_CONSUMERS_PRODUCERS_CONSUMERS_H_
#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <queue>
#include <utility>
#include <boost/mpi.hpp>

void process_buffer(
    int rank,
    int num_producers,
    int num_consumers,
    std::vector<int>* buffer,
    int* count_producer,
    int* count_consumer,
    boost::mpi::communicator* world);
std::pair<int, int> calculate_work_range(
    int rank,
    int size,
    int total_tasks);
void do_producer_work(int start, int end, int rank);
void do_consumer_work(int start, int end, int rank);
void producer_consumer_seq(int num_producers,
    int num_consumers,
    std::vector<int> results);
void producer_consumer_par(int num_producers,
    int num_consumers,
    std::vector<int> results);

#endif  // TASKS_TASK_2_FILATOV_M_PRODUCERS_CONSUMERS_PRODUCERS_CONSUMERS_H_
