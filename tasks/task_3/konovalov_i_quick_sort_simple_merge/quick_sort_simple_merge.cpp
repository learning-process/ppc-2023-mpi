// Copyright 2023 Konovalov Igor
#include "task_3/konovalov_i_quick_sort_simple_merge/quick_sort_simple_merge.h"


void swap_(int* arr, int i, int j) {
  int t = arr[i];
  arr[i] = arr[j];
  arr[j] = t;
}

void quickSortRec(int n, int* arr) {
  if (n == 1 || n == 0)
    return;

  int f = arr[n / 2];
  int j = 0;
  int k = n - 1;

  while (true) {
    while (arr[j] < f) {
      j++;
    }
    while (arr[k] > f) {
      k--;
    }
    if (j >= k) {
      break;
    }
    swap_(arr, j, k);
    j++;
    k--;
  }
  quickSortRec(j, arr);
  quickSortRec(n - j, arr + j);
}

void merge(int n1, int n2, int* left, int* right, int* res) {
  int i = 0, j = 0;
  while (i < n1 && j < n2) {
    if (left[i] < right[j]) {
      res[i + j] = left[i];
      i++;
    } else {
      res[i + j] = right[j];
      j++;
    }
  }
  if (n2 != j) {
    for (; j < n2; j++) {
      res[i + j] = right[j];
    }
  } else if (n1 != i) {
    for (; i < n1; i++) {
      res[i + j] = left[i];
    }
  }
  for (int i = 0; i < n1 + n2; i++) {
    left[i] = res[i];
  }
}

void quickSortPar(int* res, size_t n) {
  int rankProc = 0;
  int numProc = 0;

  MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

  if (rankProc == 0) {
    int* arr = new int[n];
    for (int i = 0; i < n; i++) {
      arr[i] = res[i];
    }

    MPI_Status status;
    int centralChunk = (n % numProc);
    int chunk = (n - centralChunk) / numProc;
    centralChunk += chunk;

    for (int i = 1; i < numProc; ++i) {
      MPI_Send(&arr[centralChunk + chunk * (i - 1)], chunk, MPI_INT, i,
        100, MPI_COMM_WORLD);
    }
    quickSortRec(centralChunk, arr);
    for (int i = 1; i < numProc; ++i) {
      MPI_Recv(&arr[centralChunk + chunk * (i - 1)], chunk, MPI_INT, i,
        200, MPI_COMM_WORLD, &status);
    }

    for (int i = 1; i < numProc; ++i) {
      merge(centralChunk + chunk * (i - 1), chunk, arr,
        &arr[centralChunk + chunk * (i - 1)], arr);
    }

    for (int i = 0; i < n; i++) {
      res[i] = arr[i];
    }
    delete[] arr;
  } else {
    MPI_Status status;
    int* buf;
    int count;
    MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &count);
    buf = new int[count];
    MPI_Recv(buf, count, MPI_INT, 0, 100, MPI_COMM_WORLD, &status);
    quickSortRec(count, buf);
    MPI_Send(buf, count, MPI_INT, 0, 200, MPI_COMM_WORLD);
    delete[] buf;
  }
}

bool check(int n, int* arr) {
  for (int i = 0; i < n - 1; ++i) {
    if (arr[i] > arr[i + 1]) {
      return false;
    }
  }
  return true;
}
