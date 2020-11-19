// Copyright 2020 Paranicheva Alyona
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./min_in_rows.h"

TEST(Parallel_Operations_MPI, Matrix_10x10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 10;
    int cols = 10;
    std::vector<int> arr;
    if (rank == 0)
        arr = getRandomMatrix(rows, cols);
    std::vector<int> mins1 = getParallMin(arr, rows, cols);
    if (rank == 0) {
        std::vector<int> mins2 = getSequentialMin(arr, rows, cols);
        ASSERT_EQ(mins1, mins2);
    }
}

TEST(Parallel_Operations_MPI, Matrix_12x21) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 12;
    int cols = 21;
    std::vector<int> arr;
    if (rank == 0)
        arr = getRandomMatrix(rows, cols);
    std::vector<int> mins1 = getParallMin(arr, rows, cols);
    if (rank == 0) {
        std::vector<int> mins2 = getSequentialMin(arr, rows, cols);
        ASSERT_EQ(mins1, mins2);
    }
}

TEST(Parallel_Operations_MPI, Matrix_100x100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 100;
    int cols = 100;
    std::vector<int> arr;
    if (rank == 0)
        arr = getRandomMatrix(rows, cols);
    std::vector<int> mins1 = getParallMin(arr, rows, cols);
    if (rank == 0) {
        std::vector<int> mins2 = getSequentialMin(arr, rows, cols);
        ASSERT_EQ(mins1, mins2);
    }
}

TEST(Parallel_Operations_MPI, Matrix_1x5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 1;
    int cols = 5;
    std::vector<int> arr;
    if (rank == 0)
        arr = getRandomMatrix(rows, cols);
    std::vector<int> mins1 = getParallMin(arr, rows, cols);
    if (rank == 0) {
        std::vector<int> mins2 = getSequentialMin(arr, rows, cols);
        ASSERT_EQ(mins1, mins2);
    }
}

TEST(Parallel_Operations_MPI, Matrix_3x13) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 3;
    int cols = 13;
    std::vector<int> arr;
    if (rank == 0)
        arr = getRandomMatrix(rows, cols);
    std::vector<int> mins1 = getParallMin(arr, rows, cols);
    if (rank == 0) {
        std::vector<int> mins2 = getSequentialMin(arr, rows, cols);
        ASSERT_EQ(mins1, mins2);
    }
}

int main(int argc, char* argv[]) {
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
