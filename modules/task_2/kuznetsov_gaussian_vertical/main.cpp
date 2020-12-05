// Copyright 2020 Kuznetsov Nikita
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./gaussian_vertical.h"

TEST(GAUSSIAN_STRIP_VERTICAL_SCHEME, THROWS_WHEN_GENERATE_MATRIX_SIZE_LOWER_THAN_ZERO) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    const int size = -10;
    ASSERT_ANY_THROW(getMatrix(size));
  }
}

TEST(GAUSSIAN_STRIP_VERTICAL_SCHEME, SUCCESSFULLY_GENERATED_MATRIX) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    const int size = 3;
    ASSERT_NO_THROW(getMatrix(size));
  }
}

TEST(GAUSSIAN_STRIP_VERTICAL_SCHEME, CAN_GET_RESULT_WITH_RANDOM_MATRIX) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    const int size = 3;
    std::vector<double> mat = getMatrix(size);
    ASSERT_NO_THROW(sequentialMethod(mat, size));
  }
}

TEST(GAUSSIAN_STRIP_VERTICAL_SCHEME, SUCCESSFULL_SEQUENTIAL_METHOD) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    const int size = 3;
    std::vector<double> mat = { 4, 2, -1, 1, 5, 3, -2, 2, 3, 2, -3, 0 };
    std::vector<double> result = sequentialMethod(mat, size);
    std::vector<double> exRes = { -1, 3, 1 };
    ASSERT_EQ(result, exRes);
  }
}

TEST(GAUSSIAN_STRIP_VERTICAL_SCHEME, SUCCESSFULL_PARALLEL_METHOD) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int size = 3;
  std::vector<double> mat = { 4, 2, -1, 1, 5, 3, -2, 2, 3, 2, -3, 0 };
  std::vector<double> result = parallelMethod(mat, size);
  if (rank == 0) {
    std::vector<double> exRes = { -1, 3, 1 };
    ASSERT_EQ(result, exRes);
  }
}

TEST(GAUSSIAN_STRIP_VERTICAL_SCHEME, SEQUENTIAL_EQUAL_PARALLEL_METHOD) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int size = 3;
  std::vector<double> mat = { 4, 2, -1, 1, 5, 3, -2, 2, 3, 2, -3, 0 };
  std::vector<double> res_seq = sequentialMethod(mat, size);
  std::vector<double> res_par = parallelMethod(mat, size);
  if (rank == 0) {
    ASSERT_EQ(res_seq, res_par);
  }
}

TEST(GAUSSIAN_STRIP_VERTICAL_SCHEME, THROWS_WHEN_SIZE_IS_LOWER_THAN_ZERO_SEQ) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int size = -10;
  std::vector<double> mat = { 4, 2, -1, 1, 5, 3, -2, 2, 3, 2, -3, 0 };
  if (rank == 0) {
    ASSERT_ANY_THROW(sequentialMethod(mat, size));
  }
}

TEST(GAUSSIAN_STRIP_VERTICAL_SCHEME, THROWS_WHEN_SIZE_IS_LOWER_THAN_ZERO_PAR) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int size = -10;
  std::vector<double> mat = { 4, 2, -1, 1, 5, 3, -2, 2, 3, 2, -3, 0 };
  ASSERT_ANY_THROW(parallelMethod(mat, size));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
