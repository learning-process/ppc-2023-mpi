#include "vector_dot_product.h"

using namespace std;



vector<int> vector_generation(int mn, int mx, int sz) {

	vector<int> res(sz);

	for (int i = 0; i < sz; i++) { res[i] = mn + (rand() % (mx - mn + 1)); }

	return res;
}


void print_vector(vector<int> vec, int sz) {

	for (int i = 0; i < sz; i++) {
		cout << vec[i] << " ";
	}
	cout << endl;
}

int getSequentialOperations(vector<int> a, vector<int> b,
	int count_size_vector) {

	int res = 0;

	for (int i = 0; i < count_size_vector; i++) {
		res += a[i] * b[i];
	}

	return res;
}


int getParallelOperations(vector<int> global_vec_a, vector<int> global_vec_b,
	int count_size_vector) {

	int ProcNum, ProcRank;

	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	

	vector<int> counts_element(ProcNum);
	vector<int> dis(ProcNum);

	int delta     = count_size_vector / ProcNum;
	int remainder = count_size_vector % ProcNum;

	for (int i = 0; i < ProcNum; i++) {
		counts_element[i] = delta;

		if (i < remainder) {
			counts_element[i]++;
		}

		if (i > 0) dis[i] = dis[i - 1] + counts_element[i - 1];
		else dis[i] = 0;

	}

	vector<int> a_local(counts_element[ProcRank]);
	vector<int> b_local(counts_element[ProcRank]);



	MPI_Scatterv(global_vec_a.data(), counts_element.data(), dis.data(), MPI_INT, a_local.data(), counts_element[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatterv(global_vec_b.data(), counts_element.data(), dis.data(), MPI_INT, b_local.data(), counts_element[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);

	int sum = 0;
	int sum_all = 0;

	sum = getSequentialOperations(a_local, b_local, counts_element[ProcRank]);

	MPI_Reduce(&sum, &sum_all, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);


	return sum_all;

}




