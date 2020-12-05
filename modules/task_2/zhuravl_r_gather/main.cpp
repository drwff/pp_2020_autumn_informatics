// Copyright 2020 Zhuravlev Roman
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include <random>
#include "./Gather.h"

TEST(Parallel_MPI, Test_Gather_int) {
    int size, rank;
    int count = 4;
    int root = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = count*size;
    std::vector<int> rbuf(count);
    std::vector<int> res1(n);
    std::vector<int> res2(n);
    for (int i = 0; i < count; i++) {
        rbuf[i] = (rank+1)*i;
    }
    MPI_Gather(rbuf.data(), count, MPI_INT, res1.data(), count, MPI_INT, root, MPI_COMM_WORLD);
    Gather(rbuf.data(), count, MPI_INT, res2.data(), count, MPI_INT, root, MPI_COMM_WORLD);
    if (rank == root) {
        for (int i = 0; i < n; i++) {
        ASSERT_EQ(res1[i], res2[i]);
    }
    }
}

TEST(Parallel_MPI, Test_Gather_double) {
    int size, rank;
    int count = 4;
    int root = 1;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = count*size;
    std::vector<double> rbuf(count);
    std::vector<double> res1(n);
    std::vector<double> res2(n);
    for (int i = 0; i < count; i++) {
        rbuf[i] = (rank+1)*i;
    }
    MPI_Gather(rbuf.data(), count, MPI_DOUBLE, res1.data(), count, MPI_DOUBLE, root, MPI_COMM_WORLD);
    Gather(rbuf.data(), count, MPI_DOUBLE, res2.data(), count, MPI_DOUBLE, root, MPI_COMM_WORLD);
    if (rank == root) {
        for (int i = 0; i < n; i++) {
        ASSERT_EQ(res1[i], res2[i]);
    }
    }
}

TEST(Parallel_MPI, Test_Gather_float) {
    int size, rank;
    int count = 7;
    int root = 1;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = count*size;
    std::vector<float> rbuf(count);
    std::vector<float> res1(n);
    std::vector<float> res2(n);
    for (int i = 0; i < count; i++) {
        rbuf[i] = static_cast<float>((rank+1)*i);
    }
    MPI_Gather(&rbuf[0], count, MPI_FLOAT, &res1[0], count, MPI_FLOAT, root, MPI_COMM_WORLD);
    Gather(&rbuf[0], count, MPI_FLOAT, &res2[0], count, MPI_FLOAT, root, MPI_COMM_WORLD);
    if (rank == root) {
        for (int i = 0; i < n; i++) {
        ASSERT_EQ(res1[i], res2[i]);
    }
    }
}

TEST(Parallel_MPI, Test_Gather_count) {
    int size, rank;
    int count = 17;
    int root = 1;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = count*size;
    std::vector<double> rbuf(count);
    std::vector<double> res1(n);
    std::vector<double> res2(n);
    for (int i = 0; i < count; i++) {
        rbuf[i] = (rank+1)*i;
    }
    MPI_Gather(&rbuf[0], count, MPI_DOUBLE, &res1[0], count, MPI_DOUBLE, root, MPI_COMM_WORLD);
    Gather(&rbuf[0], count, MPI_DOUBLE, &res2[0], count, MPI_DOUBLE, root, MPI_COMM_WORLD);
    if (rank == root) {
        for (int i = 0; i < n; i++) {
        ASSERT_EQ(res1[i], res2[i]);
    }
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
