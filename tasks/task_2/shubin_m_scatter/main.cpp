// Copyright 2023 Shubin Mikhail
#include <gtest/gtest.h>
#include "./custom_scatter.h"

TEST(Custom_Scatter, Test_int_scatter) {
  const int VAL = 1;
  int res_c = 0, res_d = 0;
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  res_c = test_func(VAL, true);
  res_d = test_func(VAL, false);
  if (ProcRank == 0) {
    ASSERT_EQ(res_c, res_d);
  }
}

TEST(Custom_Scatter, Test_float_scatter) {
  const float VAL = 1.0f;
  float res_c = 0.0f, res_d = 0.0f;
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  res_c = test_func(VAL, true);
  res_d = test_func(VAL, false);
  if (ProcRank == 0) {
    ASSERT_EQ(res_c, res_d);
  }
}

TEST(Custom_Scatter, Test_double_scatter) {
  const double VAL = 1.0;
  double res_c = 0.0, res_d = 0.0;
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  res_c = test_func(VAL, true);
  res_d = test_func(VAL, false);
  if (ProcRank == 0) {
    ASSERT_EQ(res_c, res_d);
  }
}

TEST(Custom_Scatter, Test_int_scatter_time) {
  const int VAL = 1,
            BATCH = 1000000;
  int res_c = 0, res_d = 0;

  double times[4];
  double time_custom = 0.0, time_default = 0.0;

  int ProcRank, ProcNum;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  std::vector<int> val_vec(ProcNum * BATCH);
  for (int i = 0; i < ProcNum * BATCH; i++) { val_vec[i] = VAL; }

  if (ProcRank == 0) { times[0] = MPI_Wtime(); }
  res_c = test_func(val_vec, BATCH, true);
  if (ProcRank == 0) { times[1] = MPI_Wtime(); }
  if (ProcRank == 0) { times[2] = MPI_Wtime(); }
  res_d = test_func(val_vec, BATCH, false);
  if (ProcRank == 0) { times[3] = MPI_Wtime(); }

  if (ProcRank == 0) {
    ASSERT_EQ(res_c, res_d);
  }

  if (ProcRank == 0) {
    time_custom = abs(times[1] - times[0]);
    time_default = abs(times[3] - times[2]);
    std::cout << "Custom scatter time: " << time_custom << std::endl;
    std::cout << "Default scatter time: " << time_default << std::endl;
    std::cout << "Custom to default scatter time ratio: " <<
                 time_custom / time_default << std::endl;
  }
}

TEST(Custom_Scatter, Test_double_scatter_time) {
  const int BATCH = 1000000;
  const double VAL = 1.0;
  double res_c = 0, res_d = 0;

  double times[4];
  double time_custom = 0.0, time_default = 0.0;

  int ProcRank, ProcNum;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  std::vector<double> val_vec(ProcNum * BATCH);
  for (int i = 0; i < ProcNum * BATCH; i++) { val_vec[i] = VAL; }

  if (ProcRank == 0) { times[0] = MPI_Wtime(); }
  res_c = test_func(val_vec, BATCH, true);
  if (ProcRank == 0) { times[1] = MPI_Wtime(); }
  if (ProcRank == 0) { times[2] = MPI_Wtime(); }
  res_d = test_func(val_vec, BATCH, false);
  if (ProcRank == 0) { times[3] = MPI_Wtime(); }

  if (ProcRank == 0) {
    ASSERT_EQ(res_c, res_d);
  }

  if (ProcRank == 0) {
    time_custom = abs(times[1] - times[0]);
    time_default = abs(times[3] - times[2]);
    std::cout << "Custom scatter time: " << time_custom << std::endl;
    std::cout << "Default scatter time: " << time_default << std::endl;
    std::cout << "Custom to default scatter time ratio: " <<
                 time_custom / time_default << std::endl;
  }
}

int main(int argc, char** argv) {
    int resultCode = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    resultCode = RUN_ALL_TESTS();
    MPI_Finalize();

    return resultCode;
}
