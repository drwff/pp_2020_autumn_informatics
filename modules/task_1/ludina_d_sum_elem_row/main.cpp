// Copyright 2020 Ludina Daria
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./sum_elem_row.h"

TEST(Sum_elem_row_MPI, Matrix_test_4x4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int row = 4, col = 4;
  std::vector<int> global_vec;

  if (rank == 0) {
    global_vec = createRandomMatrix(row, col);
  }

  std::vector<int> parallel_sum = getParallelOperations(global_vec, row, col);

  if (rank == 0) {
    std::vector<int> sequential_sum = getSequentialOperations(global_vec, row, col);
    ASSERT_EQ(sequential_sum, parallel_sum);
  }
}

TEST(Sum_elem_row_MPI, Matrix_test_24x80) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int row = 24, col = 80;
  std::vector<int> global_vec;

  if (rank == 0) {
    global_vec = createRandomMatrix(row, col);
  }

  std::vector<int> parallel_sum = getParallelOperations(global_vec, row, col);

  if (rank == 0) {
    std::vector<int> sequential_sum = getSequentialOperations(global_vec, row, col);
    ASSERT_EQ(sequential_sum, parallel_sum);
  }
}

TEST(Sum_elem_row_MPI, Matrix_test_150x150) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int row = 150, col = 150;
  std::vector<int> global_vec;

  if (rank == 0) {
    global_vec = createRandomMatrix(row, col);
  }

  std::vector<int> parallel_sum = getParallelOperations(global_vec, row, col);

  if (rank == 0) {
    std::vector<int> sequential_sum = getSequentialOperations(global_vec, row, col);
    ASSERT_EQ(sequential_sum, parallel_sum);
  }
}

TEST(Sum_elem_row_MPI, Matrix_test_43x1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int row = 43, col = 1;
  std::vector<int> global_vec;

  if (rank == 0) {
    global_vec = createRandomMatrix(row, col);
  }

  std::vector<int> parallel_sum = getParallelOperations(global_vec, row, col);

  if (rank == 0) {
    std::vector<int> sequential_sum = getSequentialOperations(global_vec, row, col);
    ASSERT_EQ(sequential_sum, parallel_sum);
  }
}

TEST(Sum_elem_row_MPI, Matrix_test_1x30) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int row = 1, col = 30;
  std::vector<int> global_vec;

  if (rank == 0) {
    global_vec = createRandomMatrix(row, col);
  }

  std::vector<int> parallel_sum = getParallelOperations(global_vec, row, col);

  if (rank == 0) {
    std::vector<int> sequential_sum = getSequentialOperations(global_vec, row, col);
    ASSERT_EQ(sequential_sum, parallel_sum);
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
