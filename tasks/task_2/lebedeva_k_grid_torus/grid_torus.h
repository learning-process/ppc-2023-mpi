// Copyright 2023 Lebedeva Ksenia
#pragma once

#include <mpi.h>

int getNext(int sender, int receiver);

void sendData(void* data, int count, MPI_Datatype datatype,
              int sender, int receiver, int tag, MPI_Comm comm);

void sendDataInternal(void* data, int count, MPI_Datatype datatype,
              int sender, int receiver, int final, int tag, MPI_Comm comm);

