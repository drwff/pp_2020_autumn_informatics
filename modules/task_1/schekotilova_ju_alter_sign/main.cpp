// Copyright 2020 Schekotilova Julia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./alter_sign.h"

TEST(Parallel_Operations_MPI, Test_test) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vec;
  const int size_v = 10;
  if (rank == 0) vec = getRandomVector(size_v);
  int global = getParallelOperations(vec, size_v);
  if (rank == 0) {
    int res = getSequentialOperations(vec, size_v);
    ASSERT_EQ(global, res);
  }
}

TEST(Parallel_Operations_MPI, Test_zero_size) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vec;
  const int size_v = 0;
  if (rank == 0) vec = getRandomVector(size_v);
  int global = getParallelOperations(vec, size_v);
  if (rank == 0) {
    int res = getSequentialOperations(vec, size_v);
    ASSERT_EQ(global, res);
  }
}

TEST(Parallel_Operations_MPI, Test_one_elem) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int size_v = 1;
  std::vector<int> vec(size_v);
  if (rank == 0) vec = getRandomVector(size_v);
  int global = getParallelOperations(vec, size_v);
  if (rank == 0) {
    ASSERT_EQ(0, global);
  }
}

TEST(Parallel_Operations_MPI, Test_same_sign) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size_v = 7;
  std::vector<int>vec = {8, 24, 1, 5, 6, 11, 20};
  std::vector<int> v = {-8, -24, -1, -5, -6, -11, -20};
  int res = getParallelOperations(vec, size_v);
  int r = getParallelOperations(v, size_v);
  if (rank == 0) {
    ASSERT_EQ(0, res);
    ASSERT_EQ(0, r);
  }
}

TEST(Parallel_Operations_MPI, Test_many_elems) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vec;
  const int size_v = 100;
  if (rank == 0) vec = getRandomVector(size_v);
  int global = getParallelOperations(vec, size_v);
  if (rank == 0) {
    int res = getSequentialOperations(vec, size_v);
    ASSERT_EQ(global, res);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);
  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
