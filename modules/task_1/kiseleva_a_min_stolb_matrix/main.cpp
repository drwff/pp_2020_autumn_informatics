// Copyright 2020 Kiseleva Anastasia
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "./min_stolb.h"

TEST(Parallel_MPI, TEST_3x3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 3;
    std::vector<std::vector<int>> matrix = { {1, 2, 3}, {0, 0, 0}, {5, 6, 9} };
    std::vector<int> RRes = { 0, 0, 0 };
    std::vector<int> linm = VVconvertV(matrix, 3, 3);
    std::vector<int> res(3);
    res = Min(linm, 3, 3);
    if (rank == 0) {
        for (int i = 0; i < str; ++i)
            EXPECT_EQ(RRes[i], res[i]);
    }
}

TEST(Parallel_MPI, TEST_2x3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int stlb = 3;
    std::vector<std::vector<int>> matrix = { {0, 5, 9}, {2, 3, 9} };
    std::vector<int> RRes = { 0, 3, 9 };
    std::vector<int> linm = VVconvertV(matrix, 2, 3);
    std::vector<int> res(3);
    res = Min(linm, 2, 3);
    if (rank == 0) {
        for (int i = 0; i < stlb; ++i)
            EXPECT_EQ(RRes[i], res[i]);
    }
}

TEST(Parallel_MPI, TEST_OTR_EL) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int stlb = 2;
    std::vector<std::vector<int>> matrix = { {-1, -9}, {0, -50} };
    std::vector<int> RRes = { -1, -50 };
    std::vector<int> linm = VVconvertV(matrix, 2, 2);
    std::vector<int> res(2);
    res = Min(linm, 2, 2);
    if (rank == 0) {
        for (int i = 0; i < stlb; ++i)
            EXPECT_EQ(RRes[i], res[i]);
    }
}

TEST(Parallel_MPI, TEST_RANDOM_7x4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 7;
    int stlb = 4;
    int size = str * stlb;
    std::vector<std::vector<int>> matrix(size);
    matrix = RandomMatrix(7, 4);
    std::vector<int> linm = VVconvertV(matrix, 7, 4);
    std::vector<int> res = Min(linm, 7, 4);
}

TEST(Parallel_MPI, TEST_RANDOM_14x10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 14;
    int stlb = 10;
    int size = str * stlb;
    std::vector<std::vector<int>> matrix(size);
    matrix = RandomMatrix(14, 10);
    std::vector<int> linm = VVconvertV(matrix, 14, 10);
    std::vector<int> res = Min(linm, 14, 10);
}

TEST(Parallel_MPI, TEST_RANDOM_4x7) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 4;
    int stlb = 7;
    int size = str * stlb;
    std::vector<std::vector<int>> matrix(size);
    matrix = RandomMatrix(4, 7);
    std::vector<int> linm = VVconvertV(matrix, 4, 7);
    std::vector<int> res = Min(linm, 4, 7);
}

TEST(Parallel_MPI, TEST_RANDOM_NOT_SQUARE) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 2;
    int stlb = 3;
    int size = str * stlb;
    std::vector<std::vector<int>> matrix(size);
    matrix = RandomMatrix(2, 3);
    std::vector<int> linm = VVconvertV(matrix, 2, 3);
    std::vector<int> res = Min(linm, 2, 3);
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
