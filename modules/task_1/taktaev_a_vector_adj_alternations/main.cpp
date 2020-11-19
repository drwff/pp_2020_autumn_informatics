// Copyright 2020 Taktaev Artem
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "../../../modules/task_1/taktaev_a_vector_adj_alternations/vector_adj_alternations.h"

TEST(Parallel_Adj_Alternations_MPI, Test_Size_10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int vec_size = 10;
    std::vector<int> full_vec(vec_size);

    if (rank == 0) full_vec = createRandomVector(vec_size);
    int count_parallel = calculateAdjAlternationsParallel(full_vec, vec_size);
    if (rank == 0) {
        int count_sequential = calculateAdjAlternationsSequential(full_vec, 1, 1);
        ASSERT_EQ(count_parallel, count_sequential);
    }
}

TEST(Parallel_Adj_Alternations_MPI, Test_Size_6) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int vec_size = 6;
    std::vector<int> full_vec(vec_size);

    if (rank == 0) full_vec = createRandomVector(vec_size);
    int count_parallel = calculateAdjAlternationsParallel(full_vec, vec_size);
    if (rank == 0) {
        int count_sequential = calculateAdjAlternationsSequential(full_vec, 1, 1);
        ASSERT_EQ(count_parallel, count_sequential);
    }
}

TEST(Parallel_Adj_Alternations_MPI, Test_Size_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int vec_size = 5;
    std::vector<int> full_vec(vec_size);

    if (rank == 0) full_vec = createRandomVector(vec_size);
    int count_parallel = calculateAdjAlternationsParallel(full_vec, vec_size);
    if (rank == 0) {
        int count_sequential = calculateAdjAlternationsSequential(full_vec, 1, 1);
        ASSERT_EQ(count_parallel, count_sequential);
    }
}

TEST(Parallel_Adj_Alternations_MPI, Test_Size_15) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int vec_size = 15;
    std::vector<int> full_vec(vec_size);

    if (rank == 0) full_vec = createRandomVector(vec_size);
    int count_parallel = calculateAdjAlternationsParallel(full_vec, vec_size);
    if (rank == 0) {
        int count_sequential = calculateAdjAlternationsSequential(full_vec, 1, 1);
        ASSERT_EQ(count_parallel, count_sequential);
    }
}

TEST(Parallel_Adj_Alternations_MPI, Test_0_Size_Par) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int vec_size = 0;
    std::vector<int> full_vec(vec_size);

    ASSERT_EQ(calculateAdjAlternationsParallel(full_vec, vec_size), 0);
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
