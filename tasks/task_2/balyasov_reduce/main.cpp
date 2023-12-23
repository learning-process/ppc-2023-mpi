// Copyright 2023 Balyasov Ivan
#include <gtest/gtest.h>

#include "./my_reduce.h"

TEST(MPI_TESTS, Test_double_sum) {
  int rank = 0;
  int world_size = 0;
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &world_size);

  double value = 10.5;
  double sum = 0;
  int recv_rank = 0;
  int return_code =
      reduce(&value, &sum, 1, MPI_DOUBLE, MPI_SUM, recv_rank, comm);

  if (return_code == MPI_SUCCESS && rank == recv_rank) {
    ASSERT_EQ(sum, world_size * value);
  }
}

TEST(MPI_TESTS, Test_float_prod) {
  int rank = 0;
  int world_size = 0;
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &world_size);

  float value = 2;
  float prod = 0;
  int recv_rank = 0;
  int return_code =
      reduce(&value, &prod, 1, MPI_FLOAT, MPI_PROD, recv_rank, comm);

  if (return_code == MPI_SUCCESS && rank == recv_rank) {
    ASSERT_EQ(prod, powf(value, static_cast<float>(world_size)));
  }
}

TEST(MPI_TESTS, Test_int_lor) {
  int rank = 0;
  int world_size = 0;
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &world_size);

  int value = 0;
  if (rank == 0) value = 1;

  int lor = 0;
  int recv_rank = 0;
  int return_code = reduce(&value, &lor, 1, MPI_INT, MPI_LOR, recv_rank, comm);

  if (return_code == MPI_SUCCESS && rank == recv_rank) {
    ASSERT_EQ(lor, value);
  }
}

TEST(MPI_TESTS, Test_int_land) {
  int rank = 0;
  int world_size = 0;
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &world_size);

  int value = 1;
  if (rank == 0) value = 0;

  int lor = 0;
  int recv_rank = 0;
  int return_code = reduce(&value, &lor, 1, MPI_INT, MPI_PROD, recv_rank, comm);

  if (return_code == MPI_SUCCESS && rank == recv_rank) {
    ASSERT_EQ(lor, value);
  }
}

TEST(MPI_TESTS, Test_error_code) {
  int rank = 0;
  int world_size = 0;
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &world_size);

  int value = 10;
  int sum = 0;
  int recv_rank = world_size;
  int return_code = reduce(&value, &sum, 1, MPI_INT, MPI_PROD, recv_rank, comm);

  if (rank == 0) {
    ASSERT_EQ(MPI_ERR_ROOT, return_code);
  }
}

int main(int argc, char** argv) {
  int code = 0;
  int rank = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  }

  code = RUN_ALL_TESTS();
  MPI_Finalize();

  return code;
}
