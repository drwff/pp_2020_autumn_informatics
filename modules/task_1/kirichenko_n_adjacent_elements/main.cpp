// Copyright 2020 kirichenko Nikita
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./adjacent_elements .h"

TEST(Parallel_Operations_MPI, test_SizeVector1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> globalVec;
    const int SizeVector = 6;

    if (rank == 0) {
        globalVec = getRandomVector(SizeVector);
    }

    int GlobMin = getParallelOperations(globalVec, SizeVector);

    if (rank == 0) {
        int  MinIndex = getSequentialOperations(globalVec, SizeVector);
        ASSERT_EQ(MinIndex, GlobMin);
    }
}

TEST(Parallel_Operations_MPI, test_SizeVector2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> globalVec;
    const int  SizeVector = 23;

    if (rank == 0) {
        globalVec = getRandomVector(SizeVector);
    }

    int GlobMin = getParallelOperations(globalVec, SizeVector);

    if (rank == 0) {
        int  MinIndex = getSequentialOperations(globalVec, SizeVector);
        ASSERT_EQ(MinIndex, GlobMin);
    }
}

TEST(Parallel_Operations_MPI, test_SizeVector3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> globalVec;
    const int SizeVector = 138;

    if (rank == 0) {
        globalVec = getRandomVector(SizeVector);
    }

    int GlobMin = getParallelOperations(globalVec, SizeVector);

    if (rank == 0) {
        int  MinIndex = getSequentialOperations(globalVec, SizeVector);
        ASSERT_EQ(MinIndex, GlobMin);
    }
}

TEST(Parallel_Operations_MPI, test_SizeVector4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> globalVec;
    const int SizeVector = 2028;

    if (rank == 0) {
        globalVec = getRandomVector(SizeVector);
    }

    int GlobMin = getParallelOperations(globalVec, SizeVector);

    if (rank == 0) {
        int  MinIndex = getSequentialOperations(globalVec, SizeVector);
        ASSERT_EQ(MinIndex, GlobMin);
    }
}

TEST(Parallel_Operations_MPI, test_SizeVector5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> globalVec;
    const int SizeVector = 3605;

    if (rank == 0) {
        globalVec = getRandomVector(SizeVector);
    }

    int GlobMin = getParallelOperations(globalVec, SizeVector);

    if (rank == 0) {
        int MinIndex = getSequentialOperations(globalVec, SizeVector);
        ASSERT_EQ(MinIndex, GlobMin);
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
