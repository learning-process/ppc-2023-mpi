#include <iostream> 
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_1/vetoshnikova_k_vector_dot_product/vector_dot_product.h"


using namespace std;

vector<int> vector_generation(int n) {

	vector<int> res_vec(n);

	srand(time(NULL));

	for (int i = 0; i < n; i++) { res_vec[i] = rand() % 100; }

	return res_vec;
}

int getParallelOperations(vector<int> global_vec_a, vector<int> global_vec_b, 
						   int count_size_vector) {

	boost::mpi::communicator world;

	int ProcNum  = world.size();
	int ProcRank = world.rank();

	boost::mpi::broadcast(world, count_size_vector, 0);

	int delta = count_size_vector / ProcNum;

	if (ProcRank != 0) {
		vector<int> a(delta);
		vector<int> b(delta);
	}

	boost::mpi::scatter(world, global_vec_a, a, 0);

}