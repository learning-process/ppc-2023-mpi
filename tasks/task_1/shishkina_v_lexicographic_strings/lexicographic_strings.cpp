// Copyright 2023 Shishkina Valeria
#include "task_1/shishkina_v_lexicographic_strings/lexicographic_strings.h"

int isLexicograpic(char* str1, char* str2) {
  int local_result = 0;
  if (strcmp(str1, str2) <= 0)
    local_result = 1;
  else
    local_result = 0;

  return local_result;
}

int parallelLexicographicStrings(char* str1, char* str2) {
  MPI_Comm comm = MPI_COMM_WORLD;

  int size;
  int rank;
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  int lenStr1 = strlen(str1);
  int lenStr2 = strlen(str2);

  int* sendcounts = new int[size];
  int remainder = std::max(lenStr1, lenStr2) % size;
  int portion = std::max(lenStr1, lenStr2) / size;
  for (int i = 0; i < size; i++) {
    sendcounts[i] = portion;
    if (i < remainder) {
      sendcounts[i]++;
    }
  }

  int* displ = new int[size];
  int _displ = 0;
  for (int i = 0; i < size; i++) {
    displ[i] = _displ;
    _displ += sendcounts[i];
  }

  int local_count1 = sendcounts[rank];
  int local_count2 = sendcounts[rank];

  char* local_str1 = new char[local_count1];
  char* local_str2 = new char[local_count2];

  MPI_Scatterv(str1, sendcounts, displ, MPI_CHAR, local_str1, local_count1,
               MPI_CHAR, 0, comm);
  MPI_Scatterv(str2, sendcounts, displ, MPI_CHAR, local_str2, local_count2,
               MPI_CHAR, 0, comm);

  int local_result = isLexicograpic(str1, str2);
  int global_result;
  MPI_Reduce(&local_result, &global_result, 1, MPI_INT, MPI_LAND, 0, comm);

  return global_result;
}
