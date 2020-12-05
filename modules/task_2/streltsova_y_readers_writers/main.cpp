// Copyright 2020 Streltsova Yana
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include "../../../modules/task_2/streltsova_y_readers_writers/readers_writers.h"

TEST(Parallel_Operations_MPI, Test_1) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int count;
    int n1 = 1;
    int n2 = 3;
    if (rank == 0)
        count = accessData(n2 * (size / 2) + n1 * ((size - 1) / 2));
    wait(0.3);
    if (rank != 0) {
        if (rank % 2 == 0) {
            for (int i = 0; i < n1; i++)
                writer();
        } else {
            for (int i = 0; i < n2; i++)
                reader();
        }
    }
    if (rank == 0) {
        ASSERT_EQ(count, 0);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test_2) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int count;
    int n1 = 2;
    int n2 = 2;
    if (rank == 0)
        count = accessData(n2 * (size / 2) + n1 * ((size - 1) / 2));
    wait(0.3);
    if (rank != 0) {
        if (rank % 2 == 0) {
            for (int i = 0; i < n1; i++)
                writer();
        } else {
            for (int i = 0; i < n2; i++)
                reader();
        }
    }
    if (rank == 0) {
        ASSERT_EQ(count, 0);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test_3) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int count;
    int n1 = 4;
    int n2 = 3;
    if (rank == 0)
        count = accessData(n2 * (size / 2) + n1 * ((size - 1) / 2));
    wait(0.3);
    if (rank != 0) {
        if (rank % 2 == 0) {
            for (int i = 0; i < n1; i++)
                writer();
        } else {
            for (int i = 0; i < n2; i++)
                reader();
        }
    }
    if (rank == 0) {
        ASSERT_EQ(count, 0);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test_4) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int count;
    int n1 = 7;
    int n2 = 8;
    if (rank == 0)
        count = accessData(n2 * (size / 2) + n1 * ((size - 1) / 2));
    wait(0.3);
    if (rank != 0) {
        if (rank % 2 == 0) {
            for (int i = 0; i < n1; i++)
                writer();
        } else {
            for (int i = 0; i < n2; i++)
                reader();
        }
    }
    if (rank == 0) {
        ASSERT_EQ(count, 0);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test_5) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int count;
    int n1 = 12;
    int n2 = 24;
    if (rank == 0)
        count = accessData(n2 * (size / 2) + n1 * ((size - 1) / 2));
    wait(0.3);
    if (rank != 0) {
        if (rank % 2 == 0) {
            for (int i = 0; i < n1; i++)
                writer();
        } else {
            for (int i = 0; i < n2; i++)
                reader();
        }
    }
    if (rank == 0) {
        ASSERT_EQ(count, 0);
    }
    MPI_Barrier(MPI_COMM_WORLD);
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
