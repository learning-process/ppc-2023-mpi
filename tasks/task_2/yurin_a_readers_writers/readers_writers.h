// Copyright 2023 Yurin Andrey
#ifndef TASKS_TASK_2_YURIN_A_READERS_WRITERS_READERS_WRITERS_H_
#define TASKS_TASK_2_YURIN_A_READERS_WRITERS_READERS_WRITERS_H_

#include <mpi.h>
#include <utility>


void Controller(const size_t& readerCount, const size_t& writerCount);
void Writer();
void Reader();
void ReadersWriters(const std::size_t readerCount, const std::size_t writerCount);

#endif  // TASKS_TASK_2_YURIN_A_READERS_WRITERS_READERS_WRITERS_H_
