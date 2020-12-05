// Copyright 2020 Mishina Nadezhda
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <random>
#include "./star.h"

TEST(Parallel_Operations_MPI, Test_0_send_rand) {
    int rank;
    int size;
    int sender;
    int reciever;
    MPI_Comm newcomm;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    newcomm = StarComm(size);
    if (size == 1) {
        ASSERT_EQ(1, 1);
    } else {
        std::mt19937 gen;
        std::uniform_int_distribution<> range{ 1, size - 1};
        sender = 0;
        reciever = range(gen);
        int s = 13;
        int r;
        if (rank == sender) {
            MPI_Send(&s, 1, MPI_INT, reciever, 0, newcomm);
        }
        if (rank == reciever) {
            MPI_Status status;
            MPI_Recv(&r, 1, MPI_INT, sender, 0, newcomm, &status);
            ASSERT_EQ(s, r);
        }
    }
}
TEST(Parallel_Operations_MPI, Test_rand_send_0) {
    int rank;
    int size;
    int sender;
    int reciever;
    MPI_Comm newcomm;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    newcomm = StarComm(size);
    if (size == 1) {
        ASSERT_EQ(1, 1);
    } else {
        std::mt19937 gen;
        std::uniform_int_distribution<> range{ 1, size - 1 };
        sender = range(gen);
        reciever = 0;
        int s = 13;
        int r;
        if (rank == sender) {
            MPI_Send(&s, 1, MPI_INT, reciever, 0, newcomm);
        }
        if (rank == reciever) {
            MPI_Status status;
            MPI_Recv(&r, 1, MPI_INT, sender, 0, newcomm, &status);
            ASSERT_EQ(s, r);
        }
    }
}
TEST(Parallel_Operations_MPI, Test_rand_send_1) {
    int rank;
    int size;
    int sender;
    int reciever;
    MPI_Comm newcomm;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    newcomm = StarComm(size);
    if ((size == 1) || (size == 2)) {
        ASSERT_EQ(1, 1);
    } else {
        std::mt19937 gen;
        std::uniform_int_distribution<> range{ 2, size - 1 };
        sender = range(gen);
        reciever = 1;
        int s = 13;
        int r;
        if (rank == sender) {
            MPI_Send(&s, 1, MPI_INT, reciever, 0, newcomm);
        }
        if (rank == reciever) {
            MPI_Status status;
            MPI_Recv(&r, 1, MPI_INT, sender, 0, newcomm, &status);
            ASSERT_EQ(s, r);
        }
    }
}
TEST(Parallel_Operations_MPI, Test_0_send_rand_double) {
    int rank;
    int size;
    int sender;
    int reciever;
    MPI_Comm newcomm;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    newcomm = StarComm(size);
    if (size == 1) {
        ASSERT_EQ(1, 1);
    } else {
        std::mt19937 gen;
        std::uniform_int_distribution<> range{ 1, size - 1 };
        sender = 0;
        reciever = range(gen);
        double s = 13.125;
        double r;
        if (rank == sender) {
            MPI_Send(&s, 1, MPI_DOUBLE, reciever, 0, newcomm);
        }
        if (rank == reciever) {
            MPI_Status status;
            MPI_Recv(&r, 1, MPI_DOUBLE, sender, 0, newcomm, &status);
            ASSERT_EQ(s, r);
        }
    }
}
TEST(Parallel_Operations_MPI, Test_0_send_rand_char) {
    int rank;
    int size;
    int sender;
    int reciever;
    MPI_Comm newcomm;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    newcomm = StarComm(size);
    if (size == 1) {
        ASSERT_EQ(1, 1);
    } else {
        std::mt19937 gen;
        std::uniform_int_distribution<> range{ 1, size - 1 };
        sender = 0;
        reciever = range(gen);
        char s = 'a';
        char r;
        if (rank == sender) {
            MPI_Send(&s, 1, MPI_CHAR, reciever, 0, newcomm);
        }
        if (rank == reciever) {
            MPI_Status status;
            MPI_Recv(&r, 1, MPI_CHAR, sender, 0, newcomm, &status);
            ASSERT_EQ(s, r);
        }
    }
}
int main(int argc, char**argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
