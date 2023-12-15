// Copyright 2023 Mortina Anastasia

#include "task_1/mortina_a_average_of_vector_elements/alg.h"

int generateRandomNumbers(int min, int max) {
  int randomNumber = min + (std::rand() % (max - min + 1));

  return randomNumber;
}

double sumOfVectorSequential(const std::vector<int>& vector) {
  double sum = 0;
  for (int i = 0; i < vector.size(); i++) {
    sum += vector[i];
  }
  return sum;
}

std::vector<int> getRandomVector(int size) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> vector(size);
  for (int i = 0; i < size; i++) {
    vector[i] = gen() % 10;
  }
  return vector;
}

double midValueOfVectorParallel(const std::vector<int>& vector) { 
  boost::mpi::communicator comm;

  int rank = comm.rank();
  int size = comm.size();
  double localSum = 0.0;
  double globalSum = 0.0;

  std::vector<int> sendCounts(size);
  std::vector<int> displacements(size);

  int chunk = static_cast<int>(vector.size() / size);
  int remainder = static_cast<int>(vector.size() % size);

  boost::mpi::all_gather(comm, chunk, sendCounts);

  displacements[0] = 0;
  for (int i = 1; i < size; i++) {
    displacements[i] = displacements[i - 1] + sendCounts[i - 1];
  }

  sendCounts[size - 1] += remainder;

  std::vector<int> recv_data(sendCounts[rank]);
  boost::mpi::scatterv(comm, vector.data(), sendCounts, displacements, recv_data.data(), sendCounts[rank], 0);

  localSum = sumOfVectorSequential(recv_data);
  boost::mpi::reduce(comm, localSum, globalSum, std::plus<int>(), 0);

  return globalSum / static_cast<double>(vector.size());
}
