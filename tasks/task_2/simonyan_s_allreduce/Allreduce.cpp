// Copyright 2023 Suren Simonyan


#include "quicksort.h"

void quickSortFunction(double* array, int left, int right) {
	if (left < right) {
		int i = left;
		int j = right;
		double pivot = array[(left + right) / 2];
		while (i <= j) {
			while (array[i] < pivot)
				i++;
			while (array[j] > pivot)
				j--;
			if (i <= j) {
				std::swap(array[i], array[j]);
				i++;
				j--;
			}
		}
		std::cout << "Before 1 Function " << "arr[0] = " << array[0] << std::endl;
		quickSortFunction(array, left, j);
		std::cout << "Before 2 Function " << "arr[0] = " << array[0] << std::endl;
		quickSortFunction(array, i, right);
	}
}

void Sort(double* arr, int n)
{
	int ProcRank, ProcNum;
	//int left, right, mid;
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	int size = n / ProcNum;
	if (ProcRank == 0)
	{
		int ost = n % ProcNum;
		int offset = 0;
		int zerosize;
		if (ost > 0) {
			zerosize = size + 1;
			ost--;
			offset++;
		}
		else {
			zerosize = size;
		}
		double* buf = new double[zerosize];
		memcpy(buf, arr, sizeof(double) * zerosize);
		for (int i = 1; i < ProcNum; ++i)
		{
			if (ost > 0)
			{
				ost--;
				MPI_Send(&arr[size * i + offset], size + 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
				offset++;
			}
			else
			{
				MPI_Send(&arr[size * i + offset], size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			}
		}
		quickSortFunction(buf, 0, size);
	}
	else
	{
		if (n % ProcNum > ProcRank)
			size++;
		double* buf = new double[size];
		MPI_Recv(buf, size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = 0; i < size; ++i)
		{
			std::cout << "Rank = " << ProcRank << "  " << buf[i] << " " << std::endl;
		}
		quickSortFunction(buf, 0, size);
	}
	//MPI_Recv()
}