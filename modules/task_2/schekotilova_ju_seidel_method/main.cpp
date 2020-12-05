// Copyright 2020 Schekotilova Julia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <numeric>
#include "./seidel_method.h"

TEST(Test_seidel_method, Test_Evklid_norma) {
  const int sz = 8;
  double eps = 0.001;
  std::vector<double> tst(sz);
  std::iota(tst.begin(), tst.end(), 0);
  ASSERT_NEAR(evklid_norma(tst), sqrt(140), eps);
}

TEST(Test_Seidel_method, Test_create_vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 4;
  std::vector<double> a(n);
  if (rank == 0) {
    ASSERT_NO_THROW(a = getRandomVector(n));
  }
}

TEST(Test_Seidel_method, Test_parallel_version) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 4;
  double eps = 0.001;
  std::vector<double>A(n * n);
  A = { 15, 3, 3, 0, 1, 14, 5, -7, -2, 1, 18, -4, 3, 3, -4, 14 };
  std::vector<double> B(n);
  B = { 3, 7, 5, 4};
  std::vector<double> x(n);
  double timeParal = MPI_Wtime();
  x = getParallelOperations(A, B, 4, eps);
  timeParal = MPI_Wtime() - timeParal;

  if (rank == 0) {
    std::vector<double> res(n);
    res = { 0.035, 0.515, 0.31, 0.256 };
    ASSERT_NEAR(evklid_norma(res), evklid_norma(x), eps);

    printf("The time: %f seconds\n", timeParal);
  }
}

TEST(Test_Seidel_method, Test_compare_time) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 8;
  double eps = 0.001;
  std::vector<double> xx(n);
  std::vector<double>A(n * n);
  A = { 75, 2, 3, 5, 7, 8, 8, 2,
      5, 52, 2, 8, 6, 4, 7, 2,
      40, 5, 88, 3, 8, 40, 8, 3,
      4, 7, 2, 98, 1, 8, 3, 8,
      6, 1, 4, 5, 65, 4, 3, 2,
      8, 2, 32, 4, 9, 98, 6, 1,
      9, 8, 7, 2, 4, 3, 52, 6,
      2, 7, 66, 8, 2, 1, 5, 98 };
  std::vector<double> B(n);
  B = { 110, 86, 123, 131, 90, 102, 91, 100};
  std::vector<double> x(n);
  double timeParal = MPI_Wtime();
  x = getParallelOperations(A, B, 8, eps);
  timeParal = MPI_Wtime() - timeParal;

  if (rank == 0) {
    std::vector<double> res(n);
    res = { 1.054, 1.031, 0.328, 1.074, 1.062, 0.61, 1.146, 0.531 };
    ASSERT_NEAR(evklid_norma(res), evklid_norma(x), eps);
    double timeSeque = MPI_Wtime();
    xx = getSequentialOperations(A, B, 8, eps);
    timeSeque = MPI_Wtime() - timeSeque;
    ASSERT_NEAR(evklid_norma(xx), evklid_norma(res), eps);
    printf("The sequential time: %f seconds\n", timeSeque);
    printf("The parallel time  : %f seconds\n", timeParal);
  }
}

TEST(Test_Seidel_method, Test_sequential_version) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int size = 4;
  double eps = 0.001;
  std::vector<double> A(size*size);
  std::vector<double> b(size);
  std::vector<double> x(size);
  std::vector<double> res(size);
  res = { 0.035, 0.515, 0.31, 0.256 };
  A[0] = 15; A[1] = 3;  A[2] = 3;  A[3] = 0;
  A[4] = 1; A[5] = 14;  A[6] = 5;  A[7] = -7;
  A[8] = -2; A[9] = 1; A[10] = 18;  A[11] = -4;
  A[12] = 3; A[13] = 3;  A[14] = -4;  A[15] = 14;
  b[0] = 3; b[1] = 7; b[2] = 5; b[3] = 4;
  if (rank == 0) {
    double timeSeque = MPI_Wtime();
    x = getSequentialOperations(A, b, 4, eps);
    timeSeque = MPI_Wtime() - timeSeque;
    ASSERT_NEAR(evklid_norma(x), evklid_norma(res), eps);
    printf("The time: %f seconds\n", timeSeque);
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
