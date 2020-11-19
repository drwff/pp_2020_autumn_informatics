// Copyright 2020 Napylov Evgenii
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./count_diff_char_mpi.h"

TEST(Parallel_Operations_MPI, Test_empty_strings) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int global_sum = getParallelDifferenceCount("", "");
    if (rank == 0) {
        int expectation = 0;
        ASSERT_EQ(expectation, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_one_empty_string) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int global_sum = getParallelDifferenceCount("", "abcde");
    if (rank == 0) {
        int expectation = 5;
        ASSERT_EQ(expectation, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_many_spaces_strings) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int global_sum = getParallelDifferenceCount("abc    de", "  abc  oe");
    if (rank == 0) {
        int expectation = 6;
        ASSERT_EQ(expectation, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_equal_strings) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int global_sum = getParallelDifferenceCount("abcde", "abcde");
    if (rank == 0) {
        int expectation = 0;
        ASSERT_EQ(expectation, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_different_strings) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int global_sum = getParallelDifferenceCount("abcde", "aYmd4");
    if (rank == 0) {
        int expectation = 3;
        ASSERT_EQ(expectation, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_different_size_strings) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int global_sum = getParallelDifferenceCount("abcde", "abFdempi");
    if (rank == 0) {
        int expectation = 4;
        ASSERT_EQ(expectation, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_random_diff_size_strings) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int global_sum = getParallelDifferenceCount(getRandomString(15), getRandomString(20));
    if (rank == 0) {
        int expectation = 0;
        ASSERT_NE(expectation, global_sum);
    }
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
