// Copyright 2023 Durandin Vladimir

#include <gtest/gtest.h>
#include "task_3/durandin_v_Strongin_global_search/Strongin_global_search.h"


TEST(Parallel_Operations_MPI, parallel_sum_in_10_elements_matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end, time_seq, time_parallel;
  if (rank == 0) {
    start = MPI_Wtime();
    global_search::seq_Strongin_alghorithm(1.0, 10.0, 0.01);
    end = MPI_Wtime();
    time_seq = end - start;
    std::cout << "Sequential " << time_seq << std::endl;
    start = MPI_Wtime();
  }
  global_search::paral_Strongin_algorithm(1.0, 10.0, 0.01);
  if (rank == 0) {
    end = MPI_Wtime();
    time_parallel = end - start;
    std::cout << "Parallel " << time_parallel << std::endl;
    std::cout << "Effective " << time_seq / time_parallel << std::endl;
  }
}

TEST(Parallel_Operations_MPI, parallel_sum_in_100_elements_matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end, time_seq, time_parallel;
  if (rank == 0) {
    start = MPI_Wtime();
    global_search::seq_Strongin_alghorithm(1.0, 100.0, 0.01);
    end = MPI_Wtime();
    time_seq = end - start;
    std::cout << "Sequential " << time_seq << std::endl;
    start = MPI_Wtime();
  }
  global_search::paral_Strongin_algorithm(1.0, 100.0, 0.01);
  if (rank == 0) {
    end = MPI_Wtime();
    time_parallel = end - start;
    std::cout << "Parallel " << time_parallel << std::endl;
    std::cout << "Effective " << time_seq / time_parallel << std::endl;
  }
}

TEST(Parallel_Operations_MPI, parallel_sum_in_1000_elements_matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end, time_seq, time_parallel;
  if (rank == 0) {
    start = MPI_Wtime();
    global_search::seq_Strongin_alghorithm(1.0, 1000.0, 0.01);
    end = MPI_Wtime();
    time_seq = end - start;
    std::cout << "Sequential " << time_seq << std::endl;
    start = MPI_Wtime();
  }
  global_search::paral_Strongin_algorithm(1.0, 1000.0, 0.01);
  if (rank == 0) {
    end = MPI_Wtime();
    time_parallel = end - start;
    std::cout << "Parallel " << time_parallel << std::endl;
    std::cout << "Effective " << time_seq / time_parallel << std::endl;
  }
}

TEST(Parallel_Operations_MPI, parallel_sum_in_500_elements_matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end, time_seq, time_parallel;
  if (rank == 0) {
    start = MPI_Wtime();
    global_search::seq_Strongin_alghorithm(1.0, 500.0, 0.01);
    end = MPI_Wtime();
    time_seq = end - start;
    std::cout << "Sequential " << time_seq << std::endl;
    start = MPI_Wtime();
  }
  global_search::paral_Strongin_algorithm(1.0, 500.0, 0.01);
  if (rank == 0) {
    end = MPI_Wtime();
    time_parallel = end - start;
    std::cout << "Parallel " << time_parallel << std::endl;
    std::cout << "Effective " << time_seq / time_parallel << std::endl;
  }
}

TEST(Parallel_Operations_MPI, parallel_sum_in_474_elements_matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end, time_seq, time_parallel;
  if (rank == 0) {
    start = MPI_Wtime();
    global_search::seq_Strongin_alghorithm(1.0, 474.0, 0.01);
    end = MPI_Wtime();
    time_seq = end - start;
    std::cout << "Sequential " << time_seq << std::endl;
    start = MPI_Wtime();
  }
  global_search::paral_Strongin_algorithm(1.0, 474.0, 0.01);
  if (rank == 0) {
    end = MPI_Wtime();
    time_parallel = end - start;
    std::cout << "Parallel " << time_parallel << std::endl;
    std::cout << "Effective " << time_seq / time_parallel << std::endl;
  }
}

TEST(Parallel_Operations_MPI, parallel_sum_in_50_elements_matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end, time_seq, time_parallel;
  if (rank == 0) {
    start = MPI_Wtime();
    global_search::seq_Strongin_alghorithm(1.0, 50.0, 0.01);
    end = MPI_Wtime();
    time_seq = end - start;
    std::cout << "Sequential " << time_seq << std::endl;
    start = MPI_Wtime();
  }
  global_search::paral_Strongin_algorithm(1.0, 50.0, 0.01);
  if (rank == 0) {
    end = MPI_Wtime();
    time_parallel = end - start;
    std::cout << "Parallel " << time_parallel << std::endl;
    std::cout << "Effective " << time_seq / time_parallel << std::endl;
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();
  if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
    MPI_Abort(MPI_COMM_WORLD, -1);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  }
  int exec_code = RUN_ALL_TESTS();
  MPI_Finalize();
  return exec_code;
}
