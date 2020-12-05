// Copyright 2020 Nikolaev Denis
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "topologiRing.h"

TEST(RingTopology, CreateRingTopology) {
    MPI_Comm ringTop = topologyRing(MPI_COMM_WORLD);
    ASSERT_TRUE(ringTop != MPI_COMM_NULL);
}

TEST(Topology_Ring, Sending_Current_Array) {
    MPI_Comm ringTop = topologyRing(MPI_COMM_WORLD);
    int ProcNum, ProcRank;
    MPI_Comm_size(ringTop, &ProcNum);
    MPI_Comm_rank(ringTop, &ProcRank);
    int array_size = 8;
    int current_rank = 0;
    int finite_rank = 2;
    int* array = new int[array_size] { 4, 1, 6, 8, 2, 14, 22, 9 };
    if (finite_rank > ProcNum - 1) {
        finite_rank = ProcNum - 1;
    }
    int* second_array = Send(array, array_size, current_rank, finite_rank, ringTop);
    if (ProcRank == finite_rank) {
        for (int i = 0; i < array_size; i++) {
            ASSERT_EQ(array[i], second_array[i]);
        }
    }
}

TEST(Topology_Ring, Current_Less_Finite) {
    MPI_Comm ringTop = topologyRing(MPI_COMM_WORLD);
    int ProcNum, ProcRank;
    MPI_Comm_size(ringTop, &ProcNum);
    MPI_Comm_rank(ringTop, &ProcRank);
    int array_size = 8;
    int current_rank = 0;
    int finite_rank = ProcNum;
    int* array = new int[array_size];
    array = randomizeArray(array_size);
    if (finite_rank > ProcNum - 1) {
        finite_rank = ProcNum - 1;
    }
    int* second_array = Send(array, array_size, current_rank, finite_rank, ringTop);
    if (ProcRank == finite_rank) {
        for (int i = 0; i < array_size; i++) {
            ASSERT_EQ(array[i], second_array[i]);
        }
    }
}

TEST(Topology_Ring, Current_More_Finite) {
    MPI_Comm ringTop = topologyRing(MPI_COMM_WORLD);
    int ProcNum, ProcRank;
    MPI_Comm_size(ringTop, &ProcNum);
    MPI_Comm_rank(ringTop, &ProcRank);
    int array_size = 15;
    int current_rank = ProcNum - 1;
    int finite_rank = 0;
    int* array = new int[array_size];
    array = randomizeArray(array_size);
    if (finite_rank > ProcNum - 1) {
        finite_rank = ProcNum - 1;
    }
    int* second_array = Send(array, array_size, current_rank, finite_rank, ringTop);
    if (ProcRank == finite_rank) {
        for (int i = 0; i < array_size; i++) {
            ASSERT_EQ(array[i], second_array[i]);
        }
    }
}

TEST(Topology_Ring, Current_Is_Finite) {
    MPI_Comm ringTop = topologyRing(MPI_COMM_WORLD);
    int ProcNum, ProcRank;
    MPI_Comm_size(ringTop, &ProcNum);
    MPI_Comm_rank(ringTop, &ProcRank);
    int array_size = 15;
    int current_rank = 0;
    int finite_rank = 0;
    int* array = new int[array_size];
    array = randomizeArray(array_size);
    if (finite_rank > ProcNum - 1) {
        finite_rank = ProcNum - 1;
    }
    int* second_array = Send(array, array_size, current_rank, finite_rank, ringTop);
    if (ProcRank == finite_rank) {
        for (int i = 0; i < array_size; i++) {
            ASSERT_EQ(array[i], second_array[i]);
        }
    }
}

TEST(Topology_Ring, Send_High_Array) {
    MPI_Comm ringTop = topologyRing(MPI_COMM_WORLD);
    int ProcNum, ProcRank;
    MPI_Comm_size(ringTop, &ProcNum);
    MPI_Comm_rank(ringTop, &ProcRank);
    int array_size = 2000;
    int current_rank = 0;
    int finite_rank = ProcNum - 1;
    int* array = randomizeArray(array_size);
    if (finite_rank > ProcNum - 1) {
        finite_rank = ProcNum - 1;
    }
    int* second_array = Send(array, array_size, current_rank, finite_rank, ringTop);
    if (ProcRank == finite_rank) {
        for (int i = 0; i < array_size; i++) {
            ASSERT_EQ(array[i], second_array[i]);
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
