// Copyright 2023 Pivovarov Alexey

#ifndef TASKS_TASK_2_PIVOVAROV_A_PRODUCER_CONSUMER_PRODUCER_CONSUMER_H_
#define TASKS_TASK_2_PIVOVAROV_A_PRODUCER_CONSUMER_PRODUCER_CONSUMER_H_

#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>

void producer_consumer_seq(int num_producers, int num_consumers, std::vector<int> results);
void producer_consumer_par(int num_producers, int num_consumers, std::vector<int> results);

#endif  // TASKS_TASK_2_PIVOVAROV_A_PRODUCER_CONSUMER_PRODUCER_CONSUMER_H_
