// Copyright 2023 Makhinya Danil
#ifndef TASKS_TASK_2_MAKHINYA_D_READERS_WRITERS_READERS_WRITERS_H_
#define TASKS_TASK_2_MAKHINYA_D_READERS_WRITERS_READERS_WRITERS_H_

#include <functional>

void start_task();

void run_random_order_read_and_write(int seed, uint16_t count_people);

void run_random_order_read_and_write(int seed, uint16_t count_writer, uint16_t count_reader);

int read_data();

void write_data(int data);


#endif  // TASKS_TASK_2_MAKHINYA_D_READERS_WRITERS_READERS_WRITERS_H_
