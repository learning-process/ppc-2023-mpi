// Copyright 2023 Isaev Dmitriy
#ifndef TASKS_TASK_2_ISAEV_D_READERS_WRITERS_READERS_WRITERS_H_
#define TASKS_TASK_2_ISAEV_D_READERS_WRITERS_READERS_WRITERS_H_

#include <mpi.h>
#include <utility>
#include <algorithm>
#include <iostream>

void Controller(const size_t& readerCount, const size_t& writerCount);
void Reader();
void Writer();
void ReadersWriters(const size_t readerCount,
                    const size_t writerCount);

#endif  // TASKS_TASK_2_ISAEV_D_READERS_WRITERS_READERS_WRITERS_H_
