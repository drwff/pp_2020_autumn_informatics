// Copyright 2020 Fedoseev Valera

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./MatirxLineMax.h"

TEST(Parallel_Operations_MPI, Throw_Then_AnyMatrixSize_Below_0) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> test_matrix;
  const int rows = 0;
  const int columns = 123;
  if (rank == 0) {
  ASSERT_ANY_THROW(test_matrix = getRandomMatrixinVector(rows, columns));
  }
}

TEST(Parallel_Operations_MPI, Throw_Then_AnyMatrixSize_Below_0_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> test_matrix = {1, 2, 3, 4};
  const int rows = 123;
  const int columns = 0;
  if (rank == 0) {
  ASSERT_ANY_THROW(test_matrix = getLineMaxParallel(test_matrix, rows, columns));
  }
}

TEST(Parallel_Operations_MPI, Test_MatrixSize_1x1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> test_matrix;
  const int rows = 1;
  const int columns = 1;

  if (rank == 0) {
  test_matrix = getRandomMatrixinVector(rows, columns);
  }

  std::vector<int> parallel = getLineMaxParallel(test_matrix, rows, columns);

  if (rank == 0) {
  std::vector<int> sequental = getLineMaxSequental(test_matrix, rows, columns);
  ASSERT_EQ(sequental, parallel);
  }
}

TEST(Parallel_Operations_MPI, Test_MatrixSize_5x5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> test_matrix;
  const int rows = 5;
  const int columns = 5;

  if (rank == 0) {
  test_matrix = getRandomMatrixinVector(rows, columns);
  }

  std::vector<int> parallel = getLineMaxParallel(test_matrix, rows, columns);

  if (rank == 0) {
  std::vector<int> sequental = getLineMaxSequental(test_matrix, rows, columns);
  ASSERT_EQ(sequental, parallel);
  }
}

TEST(Parallel_Operations_MPI, Test_MatrixSize_6x3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> test_matrix;
  const int rows = 6;
  const int columns = 3;

  if (rank == 0) {
  test_matrix = getRandomMatrixinVector(rows, columns);
  }

  std::vector<int> parallel = getLineMaxParallel(test_matrix, rows, columns);

  if (rank == 0) {
  std::vector<int> sequental = getLineMaxSequental(test_matrix, rows, columns);
  ASSERT_EQ(sequental, parallel);
  }
}

TEST(Parallel_Operations_MPI, Test_MatrixSize_10x30) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> test_matrix;
  const int rows = 10;
  const int columns = 30;

  if (rank == 0) {
  test_matrix = getRandomMatrixinVector(rows, columns);
  }

  std::vector<int> parallel = getLineMaxParallel(test_matrix, rows, columns);

  if (rank == 0) {
  std::vector<int> sequental = getLineMaxSequental(test_matrix, rows, columns);
  ASSERT_EQ(sequental, parallel);
  }
}

TEST(Parallel_Operations_MPI, Test_MatrixSize_100x100) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> test_matrix;
  const int rows = 100;
  const int columns = 100;

  if (rank == 0) {
  test_matrix = getRandomMatrixinVector(rows, columns);
  }

  std::vector<int> parallel = getLineMaxParallel(test_matrix, rows, columns);

  if (rank == 0) {
  std::vector<int> sequental = getLineMaxSequental(test_matrix, rows, columns);
  ASSERT_EQ(sequental, parallel);
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

