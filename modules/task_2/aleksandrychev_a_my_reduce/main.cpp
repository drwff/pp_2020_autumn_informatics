// Copyright 2020 Aleksandychev Andrey
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "./my_reduce.h"


TEST(Parallel_Count_Sentences_MPI, summ_of_INT_size) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int root = 0;
    int official_Reduce, my_Reduce = 0;
    int local_value = (rank + 1) * 3;

    MPI_Reduce(&local_value, &official_Reduce, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
    my_reduce(&local_value, &my_Reduce, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);

    if (rank == root) {
        ASSERT_EQ(official_Reduce, official_Reduce);
    }
}

TEST(Parallel_Count_Sentences_MPI, max_of_INT_size) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int root = 0;
    int official_Reduce, my_Reduce = -2147483647;
    int local_value = (rank + 1) * 3;

    MPI_Reduce(&local_value, &official_Reduce, 1, MPI_INT, MPI_MAX, root, MPI_COMM_WORLD);
    my_reduce(&local_value, &my_Reduce, 1, MPI_INT, MPI_MAX, root, MPI_COMM_WORLD);

    if (rank == root) {
        ASSERT_EQ(official_Reduce, official_Reduce);
    }
}

TEST(Parallel_Count_Sentences_MPI, min_of_INT_size) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int root = 0;
    int official_Reduce, my_Reduce = 2147483647;
    int local_value = (rank + 1) * 3;

    MPI_Reduce(&local_value, &official_Reduce, 1, MPI_INT, MPI_MIN, root, MPI_COMM_WORLD);
    my_reduce(&local_value, &my_Reduce, 1, MPI_INT, MPI_MIN, root, MPI_COMM_WORLD);

    if (rank == root) {
        ASSERT_EQ(official_Reduce, official_Reduce);
    }
}

TEST(Parallel_Count_Sentences_MPI, prod_of_INT_size) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int root = 0;
    int official_Reduce, my_Reduce = 1;
    int local_value = (rank + 1) * 3;

    MPI_Reduce(&local_value, &official_Reduce, 1, MPI_INT, MPI_PROD, root, MPI_COMM_WORLD);
    my_reduce(&local_value, &my_Reduce, 1, MPI_INT, MPI_PROD, root, MPI_COMM_WORLD);

    if (rank == root) {
        ASSERT_EQ(official_Reduce, official_Reduce);
    }
}

TEST(Parallel_Count_Sentences_MPI, summ_of_INT_list) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int count = 2;

    int root = 0;
    int* official_Reduce = new int[count];
    int* my_Reduce = new int[count];
    for (int i = 0; i < count; i++) {
        my_Reduce[i] = 0;
    }

    int* local_value = new int[count];
    for (int i = 0; i < count; i++) {
        local_value[i] = (rank + 1) * 3 * (i + 1);
    }

    MPI_Reduce(local_value, official_Reduce, count, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
    my_reduce(local_value, my_Reduce, count, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);

    if (rank == root) {
        for (int i = 0; i < count; i++) {
            ASSERT_EQ(official_Reduce[i], my_Reduce[i]);
        }
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
