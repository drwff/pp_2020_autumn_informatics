// Copyright 2020 Dobrov Pavel
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./rows_min.h"

TEST(Parallel_Operations_MPI, Matrix_16x16) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 16;
    int cols = 16;
    std::vector<int> global_vec;
    if (rank == 0) {
        global_vec = randomMatrix(cols, rows);
    }
    std::vector<int> result = getParallelOperations(global_vec, rows, cols);
    if (rank == 0) {
        std::vector<int> control_result = getSequentialOperations(global_vec, rows, cols);
        ASSERT_EQ(control_result, result);
    }
}

TEST(Parallel_Operations_MPI, Matrix_100x100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 100;
    int cols = 100;
    std::vector<int> global_vec;
    if (rank == 0) {
        global_vec = randomMatrix(cols, rows);
    }
    std::vector<int> result = getParallelOperations(global_vec, rows, cols);
    if (rank == 0) {
        std::vector<int> control_result = getSequentialOperations(global_vec, rows, cols);
        ASSERT_EQ(control_result, result);
    }
}

TEST(Parallel_Operations_MPI, Matrix_68x48) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 68;
    int cols = 48;
    std::vector<int> global_vec;
    if (rank == 0) {
        global_vec = randomMatrix(cols, rows);
    }
    std::vector<int> result = getParallelOperations(global_vec, rows, cols);
    if (rank == 0) {
        std::vector<int> control_result = getSequentialOperations(global_vec, rows, cols);
        ASSERT_EQ(control_result, result);
    }
}

TEST(Parallel_Operations_MPI, Matrix_37x1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 37;
    int cols = 1;
    std::vector<int> global_vec;
    if (rank == 0) {
        global_vec = randomMatrix(cols, rows);
    }
    std::vector<int> result = getParallelOperations(global_vec, rows, cols);
    if (rank == 0) {
        std::vector<int> control_result = getSequentialOperations(global_vec, rows, cols);
        ASSERT_EQ(control_result, result);
    }
}

TEST(Parallel_Operations_MPI, Matrix_1x37) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 1;
    int cols = 37;
    std::vector<int> global_vec;
    if (rank == 0) {
        global_vec = randomMatrix(cols, rows);
    }
    std::vector<int> result = getParallelOperations(global_vec, rows, cols);
    if (rank == 0) {
        std::vector<int> control_result = getSequentialOperations(global_vec, rows, cols);
        ASSERT_EQ(control_result, result);
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
