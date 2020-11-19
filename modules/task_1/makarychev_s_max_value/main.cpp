// Copyright 2020 Makarychev Sergey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./max_value.h"

TEST(Parallel_Operations_MPI, Test_0x0) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int rows = 0;
    const int cols = 0;
    if (rank == 0)
        global_vec = getRandomMatrix(rows, cols);
    ASSERT_ANY_THROW(getMaxElemParallel(global_vec, rows, cols));
}

TEST(Parallel_Operations_MPI, Test_28x0) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int rows = 28;
    const int cols = 0;
    if (rank == 0)
        global_vec = getRandomMatrix(rows, cols);
    ASSERT_ANY_THROW(getMaxElemParallel(global_vec, rows, cols));
}

TEST(Parallel_Operations_MPI, Test_45x1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int rows = 453;
    const int cols = 1;
    if (rank == 0)
        global_vec = getRandomMatrix(rows, cols);

    int global_max = getMaxElemParallel(global_vec, rows, cols);

    if (rank == 0) {
        int reference_max = getMaxElemSequential(global_vec);
        ASSERT_EQ(reference_max, global_max);
    }
}

TEST(Parallel_Operations_MPI, Test_1x39) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int rows = 1;
    const int cols = 39;
    if (rank == 0)
        global_vec = getRandomMatrix(rows, cols);

    int global_max = getMaxElemParallel(global_vec, rows, cols);

    if (rank == 0) {
        int reference_max = getMaxElemSequential(global_vec);
        ASSERT_EQ(reference_max, global_max);
    }
}

TEST(Parallel_Operations_MPI, Test_25x13) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int rows = 251;
    const int cols = 133;
    if (rank == 0)
        global_vec = getRandomMatrix(rows, cols);

    int global_max = getMaxElemParallel(global_vec, rows, cols);

    if (rank == 0) {
        int reference_max = getMaxElemSequential(global_vec);
        ASSERT_EQ(reference_max, global_max);
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
