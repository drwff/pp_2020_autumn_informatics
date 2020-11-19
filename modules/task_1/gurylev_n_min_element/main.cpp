// Copyright 2020 Gurylev Nikita
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./min_element.h"

TEST(Parallel_Operations_MPI, Test_Matrix_0x0) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int row = 0, column = 0;
    if (rank == 0) {
        global_vec = getRandomMatrix(row, column);
        ASSERT_ANY_THROW(getParallelMin(global_vec, row, column));
    }
}

TEST(Parallel_Operations_MPI, Test_Matrix_10x10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int row = 10, column = 10;

    if (rank == 0) {
        global_vec = getRandomMatrix(row, column);
    }

    int global_min = getParallelMin(global_vec, row, column);

    if (rank == 0) {
        int reference_min = getSequentialMin(global_vec);
        ASSERT_EQ(reference_min, global_min);
    }
}

TEST(Parallel_Operations_MPI, Test_Matrix_50x50) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int row = 50, column = 50;

    if (rank == 0) {
        global_vec = getRandomMatrix(row, column);
    }

    int global_min = getParallelMin(global_vec, row, column);

    if (rank == 0) {
        int reference_min = getSequentialMin(global_vec);
        ASSERT_EQ(reference_min, global_min);
    }
}

TEST(Parallel_Operations_MPI, Test_Matrix_67x1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int row = 67, column = 1;

    if (rank == 0) {
        global_vec = getRandomMatrix(row, column);
    }

    int global_min = getParallelMin(global_vec, row, column);

    if (rank == 0) {
        int reference_min = getSequentialMin(global_vec);
        ASSERT_EQ(reference_min, global_min);
    }
}

TEST(Parallel_Operations_MPI, Test_Matrix_1x49) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int row = 1, column = 49;

    if (rank == 0) {
        global_vec = getRandomMatrix(row, column);
    }

    int global_min = getParallelMin(global_vec, row, column);

    if (rank == 0) {
        int reference_min = getSequentialMin(global_vec);
        ASSERT_EQ(reference_min, global_min);
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
