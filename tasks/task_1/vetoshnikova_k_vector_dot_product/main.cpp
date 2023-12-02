#include <iostream> 
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

using namespace std;

vector<int> vector_generation(int n) {

	vector<int> res_vec(n);

	srand(time(NULL));

	for (int i = 0; i < n; i++) { res_vec[i] = rand() % 100; }

	return res_vec;
}

int main(int argc, char** argv) {

	boost::mpi::environment env(argc, argv);
	boost::mpi::communicator world;

	vector<int> global_vec_a;

	const int count_size_vector = 9;

	if (world.rank() == 0) {
		global_vec_a = vector_generation(count_size_vector);
	}


	int ProcNum = world.size();
	int ProcRank = world.rank();

	boost::mpi::broadcast(world, count_size_vector, 0);

	int delta = count_size_vector / ProcNum;

	if (ProcRank != 0) {
		vector<int> a(delta);
		vector<int> b(delta);
	}

	boost::mpi::scatter(world, global_vec_a, a, 0);

	std::cout << world.rank() << ": " << a << '\n';


}