// Copyright 2023 Sredneva Anastasiya
#include "task_2/sredneva_a_hypercube/hypercube.h"
#include <queue>

int degree(int n) {
	int i = 0;
	while (n > 1) {
		if (n % 2 != 0) {
			return -1;
		}
		n /= 2;
		i++;
	}
	return i;
}

std::vector <int> sm_matrix(int n) {
	int y = 0;
	std::vector <int> matrix(n * n);
	int N = degree(n);
	for (int i = 0; i < n; i++) {
		for (int k = 0; k < N; k++) {
			int l = pow(2, k);
			y = i ^ l;
			matrix[i * n + y] = 1;
		}
	}
	for (int i = 0; i < matrix.size(); i++) {
		if (matrix[i] != 1) {
			matrix[i] = 0;
		}
	}
	return matrix;
}

std::vector<int> bfs(int n, int start, int end, const std::vector<int>& matr) {
	std::vector<int> dist(n, n);
	std::vector<int> p(n, -1);
	dist[start] = 0;
	std::queue<int> q;
	q.push(start);
	while (!q.empty()) {
		int v = q.front();
		q.pop();
		std::vector<int> sp;
		for (int j = 0; j < n; j++) {
			if (matr[v * n + j] == 1) {
				sp.push_back(j);
			}
		}
		for (int u : sp) {
			if (dist[u] > dist[v] + 1) {
				p[u] = v;
				dist[u] = dist[v] + 1;
				q.push(u);
			}
		}
	}
	std::vector<int> path;
	while (end != -1) {
		path.push_back(end);
		end = p[end];
	}
	reverse(path.begin(), path.end());
	return path;
}

int next(int rank, const std::vector<int>& vec) {
	int y = 0;
	for (int i = 0; i < vec.size() - 1; i++) {
		if (vec[i] == rank) {
			y = vec[i + 1];
		}
	}
	return y;
}

int prev(int rank, const std::vector<int>& vec) {
	int y = 0;
	for (int i = 1; i < vec.size(); i++) {
		if (vec[i] == rank) {
			y = vec[i - 1];
		}
	}
	return y;
}

bool inVec(int rank, const std::vector<int>& vec) {
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] == rank) {
			return true;
		}
	}
	return false;
}

void send_data_hypercube(int data, int start, int end) {
	int ProcRank, ProcNum;
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	std::vector<int> vec;
	vec = bfs(ProcNum, start, end, sm_matrix(ProcNum));
	if (start == end) {
		return;
	}
	if (ProcRank == start) {
		MPI_Send(&data, 1, MPI_INT, next(ProcRank, vec), 0, MPI_COMM_WORLD);
	}
	else if ((ProcRank != start) && (ProcRank != end) && (inVec(ProcRank, vec) == true)) {
		MPI_Recv(&data, 1, MPI_INT, prev(ProcRank, vec), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Send(&data, 1, MPI_INT, next(ProcRank, vec), 0, MPI_COMM_WORLD);
	}
	else if (ProcRank == end) {
		MPI_Recv(&data, 1, MPI_INT, prev(ProcRank, vec), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
}
