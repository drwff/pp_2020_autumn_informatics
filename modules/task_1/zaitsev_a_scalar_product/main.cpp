// Copyright 2020 Zaitsev Andrey
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./scalar_product.h"

TEST(Parallel_scalar_product, Size_0x0) {
  int process_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

  int elements_count = 0;
  std::vector<int> vec1;
  std::vector<int> vec2;
  int real_result = 0;

  if (process_rank == 0) {
    vec1 = createRandomVector(elements_count);
    vec2 = createRandomVector(elements_count);
  }

  int parallel_result = scalar_product_parallel(vec1, vec2, elements_count);

  if (process_rank == 0) {
    real_result = scalar_product_of_part_vectors(vec1, vec2);
    ASSERT_EQ(parallel_result, real_result);
  }
}

TEST(Parallel_scalar_product, Size_1x1) {
  int process_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

  int elements_count = 20;
  std::vector<int> vec1;
  std::vector<int> vec2;
  int real_result = 0;

  if (process_rank == 0) {
    vec1 = createRandomVector(elements_count);
    vec2 = createRandomVector(elements_count);
  }

  int parallel_result = scalar_product_parallel(vec1, vec2, elements_count);

  if (process_rank == 0) {
    real_result = scalar_product_of_part_vectors(vec1, vec2);
    ASSERT_EQ(parallel_result, real_result);
  }
}

TEST(Parallel_scalar_product, Size_5x5) {
  int process_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

  int elements_count = 5;
  std::vector<int> vec1;
  std::vector<int> vec2;
  int real_result = 0;

  if (process_rank == 0) {
    vec1 = createRandomVector(elements_count);
    vec2 = createRandomVector(elements_count);
  }

  int parallel_result = scalar_product_parallel(vec1, vec2, elements_count);

  if (process_rank == 0) {
    real_result = scalar_product_of_part_vectors(vec1, vec2);
    ASSERT_EQ(parallel_result, real_result);
  }
}

TEST(Parallel_scalar_product, Size_10x10) {
  int process_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

  int elements_count = 10;
  std::vector<int> vec1;
  std::vector<int> vec2;
  int real_result = 0;

  if (process_rank == 0) {
    vec1 = createRandomVector(elements_count);
    vec2 = createRandomVector(elements_count);
  }

  int parallel_result = scalar_product_parallel(vec1, vec2, elements_count);

  if (process_rank == 0) {
    real_result = scalar_product_of_part_vectors(vec1, vec2);
    ASSERT_EQ(parallel_result, real_result);
  }
}

TEST(Parallel_scalar_product, Size_20x20) {
  int process_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

  int elements_count = 20;
  std::vector<int> vec1;
  std::vector<int> vec2;
  int real_result = 0;

  if (process_rank == 0) {
    vec1 = createRandomVector(elements_count);
    vec2 = createRandomVector(elements_count);
  }

  int parallel_result = scalar_product_parallel(vec1, vec2, elements_count);

  if (process_rank == 0) {
    real_result = scalar_product_of_part_vectors(vec1, vec2);
    ASSERT_EQ(parallel_result, real_result);
  }
}

TEST(Parallel_scalar_product, Size_40x40) {
  int process_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

  int elements_count = 40;
  std::vector<int> vec1;
  std::vector<int> vec2;
  int real_result = 0;

  if (process_rank == 0) {
    vec1 = createRandomVector(elements_count);
    vec2 = createRandomVector(elements_count);
  }

  int parallel_result = scalar_product_parallel(vec1, vec2, elements_count);

  if (process_rank == 0) {
    real_result = scalar_product_of_part_vectors(vec1, vec2);
    ASSERT_EQ(parallel_result, real_result);
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
