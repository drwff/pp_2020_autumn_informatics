// Copyright 2020 Tyurmina Alexandra
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <random>
#include <vector>
#include "../../../modules/task_2/tyurmina_a_allreduce/allreduce.h"

TEST(MPI_Allreduce, countBelowZero) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int senbuf = 0;
    int recvbuf = 0;

    ASSERT_EQ(My_Allreduce(&senbuf, &recvbuf, -1, MPI_INT, MPI_SUM, MPI_COMM_WORLD), MPI_ERR_COUNT);
}

TEST(MPI_Allreduce, errorType) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int senbuf = 0;
    int recvbuf = 0;

    ASSERT_EQ(My_Allreduce(&senbuf, &recvbuf, 1, MPI_CHAR, MPI_SUM, MPI_COMM_WORLD), MPI_ERR_TYPE);
}

TEST(MPI_Allreduce, errorOperation) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int senbuf = 0;
    int recvbuf = 0;

    ASSERT_EQ(My_Allreduce(&senbuf, &recvbuf, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD), MPI_ERR_OP);
}

TEST(MPI_Allreduce, allreduce_ints) {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size < 3)
        return;

    const int count = 4;
    std::vector<int> resultbuf = { 3, 6, -7, 12 };
    std::vector<int> recvbuf{ 0, 0, 0, 0 };
    std::vector<int> localBuf{0, 0, 0, 0};

    if (rank == 0) {
        localBuf = {1, 2, 3, 4};
    }
    if (rank == 1) {
        localBuf = { 0, 2, 0, 4 };
    }
    if (rank == 2) {
        localBuf = { 2, 2, -10, 4 };
    }

    My_Allreduce(localBuf.data(), recvbuf.data(), count, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    EXPECT_EQ(resultbuf, recvbuf);
}

TEST(MPI_Allreduce, allreduce_doubles) {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (size < 3)
        return;
    double resultbuf = -5.2;
    double localbuf = 0;
    double recvbuf = 0;

    if (rank == 0) {
        localbuf = 3;
    }
    if (rank == 1) {
        localbuf = 2.1;
    }
    if (rank == 2) {
        localbuf = -10.3;
    }

    My_Allreduce(&localbuf, &recvbuf, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    EXPECT_DOUBLE_EQ(resultbuf, recvbuf);
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
