// Copyright 2020 Molotkova Svetlana
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <array>
#include "./Dinning_philosophers.h"

TEST(MPI_Philosofers, Test_ph_1) {
    int myRank, ProcSize;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    int time = 1;
    ASSERT_NO_THROW(diningPhilosofers(myRank, ProcSize, time));
}

TEST(MPI_Philosofers, Test_ph_2) {
    int myRank, ProcSize;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    int time = 2;
    ASSERT_NO_THROW(diningPhilosofers(myRank, ProcSize, time));
}

TEST(MPI_Philosofers, Test_ph_3) {
    int myRank, ProcSize;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    int time = 3;
    ASSERT_NO_THROW(diningPhilosofers(myRank, ProcSize, time));
}

TEST(MPI_Philosofers, Test_ph_4) {
    int myRank, ProcSize;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    int time = 4;
    ASSERT_NO_THROW(diningPhilosofers(myRank, ProcSize, time));
}

TEST(MPI_Philosofers, Test_ph_5) {
    int myRank, ProcSize;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    int time = 5;
    ASSERT_NO_THROW(diningPhilosofers(myRank, ProcSize, time));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    srand(time(NULL));
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);

    return RUN_ALL_TESTS();
}

