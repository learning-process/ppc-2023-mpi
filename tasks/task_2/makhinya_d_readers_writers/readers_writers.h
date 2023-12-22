// Copyright 2023 Makhinya Danil
#ifndef TASKS_TASK_2_MAKHINYA_D_READERS_WRITERS_READERS_WRITERS_H_
#define TASKS_TASK_2_MAKHINYA_D_READERS_WRITERS_READERS_WRITERS_H_

#include <mpi.h>
#include <utility>

void run_problem_readers_writers(uint16_t readerCount, uint16_t writerCount);
void handler(uint16_t readerCount, uint16_t writerCount);

void write();
void read();


#endif  // TASKS_TASK_2_MAKHINYA_D_READERS_WRITERS_READERS_WRITERS_H_
