// Copyright 2020 Bessolitsyn Sergay
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include "./dining_philosophers.h"

TEST(Parallel_Operations_MPI, Test_task_1) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int restaurant_bill;
    int plates_for_eating = 2;
    if (rank == 0) {
        restaurant_bill = waiter((size-1) * plates_for_eating);
    }

    if (rank != 0) {
        philosopher(plates_for_eating);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_EQ(restaurant_bill, 0);
    }
}

TEST(Parallel_Operations_MPI, Test_task_2) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int restaurant_bill;
    int plates_for_eating = 4;
    if (rank == 0) {
        restaurant_bill = waiter((size-1) * plates_for_eating);
    }

    if (rank != 0) {
        philosopher(plates_for_eating);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_EQ(restaurant_bill, 0);
    }
}

TEST(Parallel_Operations_MPI, Test_task_3) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int restaurant_bill;
    int plates_for_eating = 5;
    if (rank == 0) {
        restaurant_bill = waiter((size-1) * plates_for_eating);
    }

    if (rank != 0) {
        philosopher(plates_for_eating);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_EQ(restaurant_bill, 0);
    }
}

TEST(Parallel_Operations_MPI, Test_task_4) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int restaurant_bill;
    int plates_for_eating = 9;
    if (rank == 0) {
        restaurant_bill = waiter((size-1) * plates_for_eating);
    }

    if (rank != 0) {
        philosopher(plates_for_eating);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_EQ(restaurant_bill, 0);
    }
}

TEST(Parallel_Operations_MPI, Test_task_5) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int restaurant_bill;
    int plates_for_eating = 11;
    if (rank == 0) {
        restaurant_bill = waiter((size-1) * plates_for_eating);
    }

    if (rank != 0) {
        philosopher(plates_for_eating);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_EQ(restaurant_bill, 0);
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
