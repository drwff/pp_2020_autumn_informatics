// Copyright 2020 Makarov Alexander
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "./string_compare.h"

TEST(Parallel_Operations_MPI, Test_100_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string global_str1, global_str2;
    const size_t str_size1 = 100;
    const size_t str_size2 = 100;

    if (rank == 0) {
        global_str1 = getRandomString(str_size1);
        global_str2 = getRandomString(str_size2);
    }

    int global_result = compareParallel(global_str1, global_str2, str_size1,
                                        str_size2);
    if (rank == 0) {
        int reference_result = compareSequential(global_str1, global_str2);
        ASSERT_EQ(reference_result, global_result);
    }
}

TEST(Parallel_Operations_MPI, Test_200_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string global_str1, global_str2;
    const size_t str_size1 = 200;
    const size_t str_size2 = 100;

    if (rank == 0) {
        global_str1 = getRandomString(str_size1);
        global_str2 = getRandomString(str_size2);
    }

    int global_result = compareParallel(global_str1, global_str2, str_size1,
                                        str_size2);
    if (rank == 0) {
        int reference_result = compareSequential(global_str1, global_str2);
        ASSERT_EQ(reference_result, global_result);
    }
}

TEST(Parallel_Operations_MPI, Test_0_0) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string global_str1, global_str2;
    const size_t str_size1 = 200;
    const size_t str_size2 = 100;

    if (rank == 0) {
        global_str1 = getRandomString(str_size1);
        global_str2 = getRandomString(str_size2);
    }

    int global_result = compareParallel(global_str1, global_str2, str_size1,
                                        str_size2);
    if (rank == 0) {
        int reference_result = compareSequential(global_str1, global_str2);
        ASSERT_EQ(reference_result, global_result);
    }
}

TEST(Parallel_Operations_MPI, Test_10000_10000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string global_str1, global_str2;
    const size_t str_size1 = 200;
    const size_t str_size2 = 100;

    if (rank == 0) {
        global_str1 = getRandomString(str_size1);
        global_str2 = getRandomString(str_size2);
    }

    int global_result = compareParallel(global_str1, global_str2, str_size1,
                                        str_size2);
    if (rank == 0) {
        int reference_result = compareSequential(global_str1, global_str2);
        ASSERT_EQ(reference_result, global_result);
    }
}

TEST(Parallel_Operations_MPI, Test_My_Strings) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string global_str1 = "hello, world!";
    std::string global_str2 = "world, hello!";
    const size_t str_size1 = 13;
    const size_t str_size2 = 13;

    if (rank == 0) {
        global_str1 = getRandomString(str_size1);
        global_str2 = getRandomString(str_size2);
    }

    int global_result = compareParallel(global_str1, global_str2, str_size1,
                                        str_size2);
    if (rank == 0) {
        int reference_result = compareSequential(global_str1, global_str2);
        ASSERT_EQ(reference_result, global_result);
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
