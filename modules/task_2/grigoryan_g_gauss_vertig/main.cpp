// Copyright 2020 Grigoryan Garry
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_2/grigoryan_g_gauss_vertig/gauss_vertig.h"

TEST(gauss_vertig, correct_size) {
  std::vector<double> matrix;
  ASSERT_ANY_THROW(matrix = RandomMatrix(-3, time(0)));
}


TEST(gauss_vertig, gauss_right0) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> matrix = { 1, 0, 0, 0, 1, 1, 0, 0, 6, 2, 2, -4, 2, 2, 1, 2, 3, 4, 2, 0 };
  std::vector<double> vector(4);
  vector = OGauss(matrix, 4);


  if (rank == 0) {
    std::vector<double> real_result = { -5, 2, 1, 0 };
    ASSERT_EQ(vector, real_result);
  }
}

TEST(gauss_vertig, gauss_right1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> matrix = { 5, 0, 1, 1, 2, 0, 0, -2, 7, 1, 2, 10 };
  std::vector<double> res = ParrG(matrix, 3);


  if (rank == 0) {
    std::vector<double> real_result = { 5, 0, 0, 1, 2, 0, 0, -2, 6.8, 1, 2, 10 };
    ASSERT_EQ(res, real_result);
  }
}

TEST(gauss_vertig, gauss_right2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  std::vector<double> matrix = { 3, 2, -5, -1, 2, -1, 3, 13, 1, 2, -1, 9 };
  std::vector<double> res = ParrG(matrix, 3);

  if (rank == 0) {
    std::vector<double> real_result = { -5, 0, 0, -1, 1.6, 0, 1, 13.4, 17, 9, 2.6, 10 };
    ASSERT_EQ(res, real_result);
  }
}



TEST(gauss_vertig, create_matrix) {
  std::vector<double> matrix;
  ASSERT_NO_THROW(matrix = RandomMatrix(9, time(0)));
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

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  return RUN_ALL_TESTS();
}
