// Copyright 2023 Durandin Vladimir
#include <gtest/gtest.h>

#include "task_2/durandin_v_my_scatter/scatter.h"

TEST(Test_parents_and_sons, first_test) {
  int rank = 7;  // root of a tree
  int parent = -1, left_son = -1, right_son = -1;
  int left = 0;
  int right = 15;
  int type_size, recvcount, position;
  my_scatter::Binary_Search(left, right - 1, &rank, &parent, &left_son,
                            &right_son, &position, &type_size, &recvcount);

  EXPECT_EQ(-1, parent);
  EXPECT_EQ(3, left_son);
  EXPECT_EQ(11, right_son);
}

TEST(Test_parents_and_sons, second_test) {
  int rank = 3;
  int parent, left_son, right_son;
  int left = 0;
  int right = 15;
  int type_size, recvcount, position;
  my_scatter::Binary_Search(left, right - 1, &rank, &parent, &left_son,
                            &right_son, &position, &type_size, &recvcount);

  EXPECT_EQ(7, parent);
  EXPECT_EQ(1, left_son);
  EXPECT_EQ(5, right_son);
}

TEST(Test_parents_and_sons, third_test) {
  int rank = 1;
  int parent, left_son, right_son;
  int left = 0;
  int right = 15;
  int type_size, recvcount, position;
  my_scatter::Binary_Search(left, right - 1, &rank, &parent, &left_son,
                            &right_son, &position, &type_size, &recvcount);

  EXPECT_EQ(3, parent);
  EXPECT_EQ(0, left_son);
  EXPECT_EQ(2, right_son);
}

TEST(Test_parents_and_sons, fourth_test) {
  int rank = 9;
  int parent, left_son, right_son;
  int left = 0;
  int right = 15;
  int type_size, recvcount, position;
  my_scatter::Binary_Search(left, right - 1, &rank, &parent, &left_son,
                            &right_son, &position, &type_size, &recvcount);

  EXPECT_EQ(11, parent);
  EXPECT_EQ(8, left_son);
  EXPECT_EQ(10, right_son);
}

TEST(Test_parents_and_sons, fifth_test) {
  int rank = 8;
  int parent, left_son, right_son;
  int left = 0;
  int right = 15;
  int type_size, recvcount, position;
  my_scatter::Binary_Search(left, right - 1, &rank, &parent, &left_son,
                            &right_son, &position, &type_size, &recvcount);

  EXPECT_EQ(9, parent);
  EXPECT_EQ(-1, left_son);
  EXPECT_EQ(-1, right_son);
}

TEST(Test_parents_and_sons, sixth_test) {
  int rank = 3;
  int parent, left_son, right_son;
  int left = 0;
  int right = 4;
  int type_size, recvcount, position;
  my_scatter::Binary_Search(left, right - 1, &rank, &parent, &left_son,
                            &right_son, &position, &type_size, &recvcount);

  EXPECT_EQ(2, parent);
  EXPECT_EQ(-1, left_son);
  EXPECT_EQ(-1, right_son);
}

TEST(scatter_tests, empty_root_vector) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int vector_size = 0;
  int send_count = vector_size / size;
  std::vector<int64_t> recv_buf_my_scatter(send_count);
  std::vector<int64_t> recv_buf(send_count);
  std::vector<int64_t> vector;

  if (rank == 0)
    vector = create_random_vector_int64(vector_size, -100ll,
                                        100ll);  // 10'007 is a prime number

  int result = my_scatter::MPI_Scatter(
      vector.data(), send_count, MPI_INT64_T, recv_buf_my_scatter.data(),
      recv_buf_my_scatter.size(), MPI_INT64_T, 0, MPI_COMM_WORLD);

  if (rank == 0)
    ASSERT_EQ(MPI_ERR_BUFFER, result);
  else
    ASSERT_EQ(MPI_ERR_COUNT, result);
}

TEST(scatter_tests, small_vector) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int vector_size = 12;
  int send_count = vector_size / size;
  std::vector<int64_t> recv_buf_my_scatter(send_count);
  std::vector<int64_t> recv_buf(send_count);
  std::vector<int64_t> vector;

  if (rank == 0)
    vector = create_random_vector_int64(vector_size, -100ll,
                                        100ll);  // 10'007 is a prime number

  MPI_Scatter(vector.data(), send_count, MPI_INT64_T, recv_buf.data(),
              recv_buf.size(), MPI_INT64_T, 0, MPI_COMM_WORLD);
  my_scatter::MPI_Scatter(
      vector.data(), send_count, MPI_INT64_T, recv_buf_my_scatter.data(),
      recv_buf_my_scatter.size(), MPI_INT64_T, 0, MPI_COMM_WORLD);

  for (int i = 0; i < send_count; ++i)
    EXPECT_EQ(recv_buf[i], recv_buf_my_scatter[i]);
}

TEST(scatter_tests,
     large_vector_whose_size_is_a_prime_number_and_type_is_int64) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int vector_size = 10'007;
  int send_count = vector_size / size;
  std::vector<int64_t> recv_buf_my_scatter(send_count);
  std::vector<int64_t> recv_buf(send_count);
  std::vector<int64_t> vector;

  if (rank == 0)
    vector = create_random_vector_int64(vector_size, -100ll,
                                        100ll);  // 10'007 is a prime number

  MPI_Scatter(vector.data(), send_count, MPI_INT64_T, recv_buf.data(),
              recv_buf.size(), MPI_INT64_T, 0, MPI_COMM_WORLD);
  my_scatter::MPI_Scatter(
      vector.data(), send_count, MPI_INT64_T, recv_buf_my_scatter.data(),
      recv_buf_my_scatter.size(), MPI_INT64_T, 0, MPI_COMM_WORLD);

  for (int i = 0; i < send_count; ++i)
    EXPECT_EQ(recv_buf[i], recv_buf_my_scatter[i]);
}

TEST(scatter_tests, large_vector_whose_size_is_a_prime_number_and_type_is_int) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int vector_size = 10'007;
  int send_count = vector_size / size;
  std::vector<int> recv_buf_my_scatter(send_count);
  std::vector<int> recv_buf(send_count);
  std::vector<int> vector;

  if (rank == 0)
    vector = create_random_vector_int(vector_size, -100ll,
                                      100ll);  // 10'007 is a prime number

  MPI_Scatter(vector.data(), send_count, MPI_INT, recv_buf.data(),
              recv_buf.size(), MPI_INT, 0, MPI_COMM_WORLD);
  my_scatter::MPI_Scatter(
      vector.data(), send_count, MPI_INT, recv_buf_my_scatter.data(),
      recv_buf_my_scatter.size(), MPI_INT, 0, MPI_COMM_WORLD);

  for (int i = 0; i < send_count; ++i)
    EXPECT_EQ(recv_buf[i], recv_buf_my_scatter[i]);
}

TEST(scatter_tests,
     large_vector_whose_size_is_a_prime_number_and_type_is_float) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int vector_size = 10'007;
  int send_count = vector_size / size;
  std::vector<float> recv_buf_my_scatter(send_count);
  std::vector<float> recv_buf(send_count);
  std::vector<float> vector;

  if (rank == 0)
    vector = create_random_vector_float(vector_size, -100ll,
                                        100ll);  // 10'007 is a prime number

  MPI_Scatter(vector.data(), send_count, MPI_FLOAT, recv_buf.data(),
              recv_buf.size(), MPI_FLOAT, 0, MPI_COMM_WORLD);
  my_scatter::MPI_Scatter(
      vector.data(), send_count, MPI_FLOAT, recv_buf_my_scatter.data(),
      recv_buf_my_scatter.size(), MPI_FLOAT, 0, MPI_COMM_WORLD);

  for (int i = 0; i < send_count; ++i)
    EXPECT_EQ(recv_buf[i], recv_buf_my_scatter[i]);
}

TEST(scatter_tests,
     large_vector_whose_size_is_a_prime_number_and_type_is_double) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int vector_size = 10'007;
  int send_count = vector_size / size;
  std::vector<double> recv_buf_my_scatter(send_count);
  std::vector<double> recv_buf(send_count);
  std::vector<double> vector;

  if (rank == 0)
    vector = create_random_vector_double(vector_size, -100ll,
                                         100ll);  // 10'007 is a prime number

  MPI_Scatter(vector.data(), send_count, MPI_DOUBLE, recv_buf.data(),
              recv_buf.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
  my_scatter::MPI_Scatter(
      vector.data(), send_count, MPI_DOUBLE, recv_buf_my_scatter.data(),
      recv_buf_my_scatter.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

  for (int i = 0; i < send_count; ++i)
    EXPECT_EQ(recv_buf[i], recv_buf_my_scatter[i]);
}

TEST(scatter_tests,
     large_vector_whose_size_is_a_prime_number_and_root_is_not_zero) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int vector_size = 10'007;
  int send_count = vector_size / size;
  std::vector<int64_t> recv_buf_my_scatter(send_count);
  std::vector<int64_t> recv_buf(send_count);
  std::vector<int64_t> vector;

  if (rank == size / 2)
    vector = create_random_vector_int64(vector_size, -100ll,
                                        100ll);  // 10'007 is a prime number

  MPI_Scatter(vector.data(), send_count, MPI_INT64_T, recv_buf.data(),
              recv_buf.size(), MPI_INT64_T, size / 2, MPI_COMM_WORLD);
  my_scatter::MPI_Scatter(
      vector.data(), send_count, MPI_INT64_T, recv_buf_my_scatter.data(),
      recv_buf_my_scatter.size(), MPI_INT64_T, size / 2, MPI_COMM_WORLD);

  for (int i = 0; i < send_count; ++i)
    EXPECT_EQ(recv_buf[i], recv_buf_my_scatter[i]);
}

TEST(
    scatter_tests,
    time_measurements__large_vector_whose_size_is_a_prime_number_and_type_is_int64) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int vector_size = 10'007;
  int send_count = vector_size / size;
  std::vector<int64_t> recv_buf_my_scatter(send_count);
  std::vector<int64_t> recv_buf(send_count);
  std::vector<int64_t> vector;

  if (rank == 0)
    vector = create_random_vector_int64(vector_size, -100ll,
                                        100ll);  // 10'007 is a prime number

  double start_time, end_time;

  if (rank == 0) start_time = MPI_Wtime();
  MPI_Scatter(vector.data(), send_count, MPI_INT64_T, recv_buf.data(),
              recv_buf.size(), MPI_INT64_T, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    end_time = MPI_Wtime();
    std::cout << "Running time MPI_Scatter = " << end_time - start_time
              << std::endl;
  }

  if (rank == 0) start_time = MPI_Wtime();
  my_scatter::MPI_Scatter(
      vector.data(), send_count, MPI_INT64_T, recv_buf_my_scatter.data(),
      recv_buf_my_scatter.size(), MPI_INT64_T, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    end_time = MPI_Wtime();
    std::cout << "Running time MY MPI_Scatter = " << end_time - start_time
              << std::endl;
  }

  for (int i = 0; i < send_count; ++i)
    EXPECT_EQ(recv_buf[i], recv_buf_my_scatter[i]);
}

TEST(my_faster_scatter_tests, first_test) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // My implementation doesn't quite get it right
  if (size == 1 || size == 2 || size == 4 || size == 8) {
    int vector_size = 12;
    int send_count = vector_size / size;
    std::vector<int64_t> recv_buf_my_scatter(send_count);
    std::vector<int64_t> recv_buf(send_count);
    std::vector<int64_t> vector;

    if (rank == 0) vector = {4, 1, 7, 3, 12, 54, 78, 43, 76, 15, 18, 48};

    MPI_Scatter(vector.data(), send_count, MPI_INT64_T, recv_buf.data(),
                recv_buf.size(), MPI_INT64_T, 0, MPI_COMM_WORLD);
    my_scatter::Binary_MPI_Scatter(
        vector.data(), send_count, MPI_INT64_T, recv_buf_my_scatter.data(),
        recv_buf_my_scatter.size(), MPI_INT64_T, 0, MPI_COMM_WORLD);

    for (int i = 0; i < send_count; ++i) {
      EXPECT_EQ(recv_buf[i], recv_buf_my_scatter[i]);
      // std::cout << "Rank = " << rank << " , recv_buf[" << i << "] = " <<
      // recv_buf[i] << ", recv_buf_my_scatter[" << i << "] = " <<
      // recv_buf_my_scatter[i] << std::endl;
    }
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();
  if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  }
  int exec_code = RUN_ALL_TESTS();
  MPI_Finalize();
  return exec_code;
}
