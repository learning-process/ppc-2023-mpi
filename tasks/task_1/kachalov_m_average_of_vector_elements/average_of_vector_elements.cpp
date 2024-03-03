// Copyright 2023 Kachalov Mikhail
#include "task_1/kachalov_m_num_of_average_of_vector_elements/average_of_vector_elements.h"
#include <random>
#include <ctime>
#include <functional>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

double seq_average_of_vector_elements(const std::vector<int>& vec) {
  double sum = 0;
  for (int i = 0; i < vec.size(); i++) {
    sum += vec[i];
  }
  return sum;
}

double parallel_average_of_vector_elements(const std::vector<int>& vec) {
  boost::mpi::communicator comm;

  int rank = comm.rank();
  int size = comm.size();
  double localSum = 0.0;
  double globalSum = 0.0;

  std::vector<int> sendCounts(size);
  std::vector<int> displacements(size);

  int chunk = static_cast<int>(vec.size() / size);
  int remainder = static_cast<int>(vec.size() % size);

  boost::mpi::all_gather(comm, chunk, sendCounts);

  displacements[0] = 0;
  for (int i = 1; i < size; i++) {
    displacements[i] = displacements[i - 1] + sendCounts[i - 1];
  }

  sendCounts[size - 1] += remainder;

  std::vector<int> recv_data(sendCounts[rank]);
  boost::mpi::scatterv(comm, vec.data(), sendCounts, displacements, recv_data.data(), sendCounts[rank], 0);

  localSum = seq_average_of_vector_elements(recv_data);
  boost::mpi::reduce(comm, localSum, globalSum, std::plus<int>(), 0);

  return globalSum / static_cast<double>(vec.size());
}

std::vector<int> get_random_vector(const int& length) {
    std::vector<int> vec(length);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < length; i++) {
        vec[i] = gen() % 100;
    }

    return vec;
}
