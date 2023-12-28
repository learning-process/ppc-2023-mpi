// Copyright 2023 Kistrimova Ekaterina
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "task_3/kistrimova_e_trapezoid_indegral/trapezoid_integral.h"

TEST(Integral_Sequential, can_solve_integral_sequential) {
  Func *f = new PiIntegral();
  ASSERT_NO_THROW(TrapezoidIntegralSequential(f, f->GetLowerBound(),
                                              f->GetUpperBound(), 10));
}

TEST(Integral_Sequential, can_solve_integral_parallel) {
  Func *f = new PiIntegral();
  ASSERT_NO_THROW(
    TrapezoidIntegralParallel(f, f->GetLowerBound(),
      f->GetUpperBound(), 10));
}

TEST(Integral_Sequential, seq_parall_is_eqv) {
  Func *f = new PiIntegral();
  int procrank;
  MPI_Comm_rank(MPI_COMM_WORLD, &procrank);
  double res_sequal = TrapezoidIntegralSequential(f, f->GetLowerBound(),
                                                  f->GetUpperBound(), 100);
  double res_paral =
      TrapezoidIntegralParallel(f, f->GetLowerBound(),
      f->GetUpperBound(), 100);

  if (procrank == 0) {
    ASSERT_TRUE(fabs(res_paral - res_sequal) < 0.01);
  }
}

TEST(Integral_Sequential, first_integral) {
  Func *f = new FirstIntegral();
  int procrank;
  MPI_Comm_rank(MPI_COMM_WORLD, &procrank);
  double res_sequal = TrapezoidIntegralSequential(f, f->GetLowerBound(),
                                                  f->GetUpperBound(), 150);
  double res_paral =
      TrapezoidIntegralParallel(f, f->GetLowerBound(),
        f->GetUpperBound(), 150);

  if (procrank == 0) {
    ASSERT_TRUE(fabs(res_paral - res_sequal) < 0.01);
  }
}

TEST(Integral_Sequential, second_integral) {
  Func *f = new SecondIntegral();
  int procrank;
  MPI_Comm_rank(MPI_COMM_WORLD, &procrank);
  double res_sequal = TrapezoidIntegralSequential(f, f->GetLowerBound(),
                                                  f->GetUpperBound(), 200);
  double res_paral =
      TrapezoidIntegralParallel(f, f->GetLowerBound(),
        f->GetUpperBound(), 200);

  if (procrank == 0) {
    ASSERT_TRUE(fabs(res_paral - res_sequal) < 0.01);
  }
}

TEST(Integral_Sequential, third_integral) {
  Func *f = new ThirdIntegral();
  int procrank;
  MPI_Comm_rank(MPI_COMM_WORLD, &procrank);

  double res_sequal = 0.0;
  if (procrank == 0) {
    res_sequal = TrapezoidIntegralSequential(f, f->GetLowerBound(),
                                             f->GetUpperBound(), 100);
  }

  MPI_Barrier(MPI_COMM_WORLD);
  double res_paral =
      TrapezoidIntegralParallel(f, f->GetLowerBound(),
        f->GetUpperBound(), 100);

  if (procrank == 0) {
    ASSERT_TRUE(fabs(res_paral - res_sequal) < 0.01);
  }
}

TEST(Integral_Sequential, time_test) {
  Func *f = new ThirdIntegral();
  int procrank;
  double start_time, end_time, seq_time, par_time;
  MPI_Comm_rank(MPI_COMM_WORLD, &procrank);

  double res_sequal = 0.0;
  if (procrank == 0) {
    start_time = MPI_Wtime();
    res_sequal = TrapezoidIntegralSequential(f, f->GetLowerBound(),
                                             f->GetUpperBound(), 100);
    end_time = MPI_Wtime();
    seq_time = end_time - start_time;
    std::cout << "time seq: " << seq_time << "\n";
    start_time = MPI_Wtime();
  }

  MPI_Barrier(MPI_COMM_WORLD);
  double res_paral =
      TrapezoidIntegralParallel(f, f->GetLowerBound(),
        f->GetUpperBound(), 100);

  if (procrank == 0) {
    end_time = MPI_Wtime();
    par_time = end_time - start_time;
    std::cout << "time par: " << par_time << "\n";
    std::cout << "parall value: " << seq_time / par_time << "\n";
    ASSERT_TRUE(fabs(res_paral - res_sequal) < 0.01);
  }
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();
  if (rank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  }
  int result = RUN_ALL_TESTS();
  MPI_Finalize();
  return result;
}
