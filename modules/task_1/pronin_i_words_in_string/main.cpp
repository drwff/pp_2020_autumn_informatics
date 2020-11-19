// Copyright 2020 Pronin Igor
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <string>
#include "./pronin_i_words_in_string.h"

TEST(Parallel_Operations_MPI, Line_Size_101) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int size = 101;
    std::string str = "";
    if (rank == 0) {
        str = RandomString(size);
    }
    int parallel = ParallelOperations(str, size);
    if (rank == 0) {
        int sequential = SequentialOperations(str);
        ASSERT_EQ(sequential, parallel);
    }
}
TEST(Parallel_Operations_MPI, Line_Size_200) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int size = 200;
    std::string str = "";
    if (rank == 0) {
        str = RandomString(size);
    }
    int parallel = ParallelOperations(str, size);
    if (rank == 0) {
        int sequential = SequentialOperations(str);
        ASSERT_EQ(sequential, parallel);
    }
}
TEST(Parallel_Operations_MPI, Line_Size_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int size = 1000;
    std::string str = "";
    if (rank == 0) {
        str = RandomString(size);
    }
    int parallel = ParallelOperations(str, size);
    if (rank == 0) {
        int sequential = SequentialOperations(str);
        ASSERT_EQ(sequential, parallel);
    }
}
TEST(Parallel_Operations_MPI, Line_Size_10000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int size = 10000;
    std::string str = "";
    if (rank == 0) {
        str = RandomString(size);
    }
    int parallel = ParallelOperations(str, size);
    if (rank == 0) {
        int sequential = SequentialOperations(str);
        ASSERT_EQ(sequential, parallel);
    }
}
TEST(Parallel_Operations_MPI, Own_string) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str = "Hello world";
    int parallel = ParallelOperations(str, str.size());
    if (rank == 0) {
        int sequential = SequentialOperations(str);
        ASSERT_EQ(sequential, parallel);
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
