// Copyright 2020 Solovev Alexandr
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <string>
#include <algorithm>
#include "./count_letters.h"

TEST(Parallel_Count_Letters_MPI, Size_1) {
    int process_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    int elements_count = 1;
    std::string result;
    if (process_rank == 0)
        result = CreateRandomString(elements_count);
    int sum = calculateCountParallel(result, elements_count);
    if (process_rank == 0) {
        int control_sum = calculateCountSequental(result);
        ASSERT_EQ(control_sum, sum);
    }
}

TEST(Parallel_Count_Letters_MPI, Size_10) {
    int process_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    int elements_count = 10;
    std::string result;
    if (process_rank == 0)
        result = CreateRandomString(elements_count);
    int sum = calculateCountParallel(result, elements_count);
    if (process_rank == 0) {
        int control_sum = calculateCountSequental(result);
        ASSERT_EQ(control_sum, sum);
    }
}

TEST(Parallel_Count_Letters_MPI, Size_100) {
    int process_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    int elements_count = 100;
    std::string result;
    if (process_rank == 0)
        result = CreateRandomString(elements_count);
    int sum = calculateCountParallel(result, elements_count);
    if (process_rank == 0) {
        int control_sum = calculateCountSequental(result);
        ASSERT_EQ(control_sum, sum);
    }
}

TEST(Parallel_Count_Letters_MPI, Size_1000) {
    int process_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    int elements_count = 1000;
    std::string result;
    if (process_rank == 0)
        result = CreateRandomString(elements_count);
    int sum = calculateCountParallel(result, elements_count);
    if (process_rank == 0) {
        int control_sum = calculateCountSequental(result);
        ASSERT_EQ(control_sum, sum);
    }
}

TEST(Parallel_Count_Letters_MPI, Size_1500) {
    int process_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    int elements_count = 1500;
    std::string result;
    if (process_rank == 0)
        result = CreateRandomString(elements_count);
    int sum = calculateCountParallel(result, elements_count);
    if (process_rank == 0) {
        int control_sum = calculateCountSequental(result);
        ASSERT_EQ(control_sum, sum);
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
