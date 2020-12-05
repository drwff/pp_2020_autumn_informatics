// Copyright 2020 Paranicheva Alyona
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "./gather.h"

TEST(Parallel_Lab2_MPI, Test_int) {
    int rank, size;
    int root = 0;
    int num = 3;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<int> vec = getRandomArrInt(num, rank);
    std::vector<int> rbuf(size * num);
    std::vector<int> mybuf(size * num);
    Gather(vec.data(), num, MPI_INT,
        mybuf.data(), num, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Gather(vec.data(), num, MPI_INT,
        rbuf.data(), num, MPI_INT, root, MPI_COMM_WORLD);
    if (rank == root) {
        ASSERT_EQ(mybuf, rbuf);
    }
}

TEST(Parallel_Lab2_MPI, Test_double) {
    int rank, size;
    int root = 0;
    int num = 3;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<double> vec = getRandomArrDouble(num, rank);
    std::vector<double> rbuf(size* num);
    std::vector<double> mybuf(size* num);
    Gather(vec.data(), num, MPI_DOUBLE,
        mybuf.data(), num, MPI_DOUBLE, root, MPI_COMM_WORLD);
    MPI_Gather(vec.data(), num, MPI_DOUBLE,
        rbuf.data(), num, MPI_DOUBLE, root, MPI_COMM_WORLD);
    if (rank == root) {
        ASSERT_EQ(mybuf, rbuf);
    }
}

TEST(Parallel_Lab2_MPI, Test_int_and_time) {
    int rank, size;
    int root = 0;
    int num = 5;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<int> vec = getRandomArrInt(num, rank);
    std::vector<int> rbuf(size * num);
    std::vector<int> mybuf(size * num);
    double mytimes = MPI_Wtime();
    Gather(vec.data(), num, MPI_INT,
        mybuf.data(), num, MPI_INT, root, MPI_COMM_WORLD);
    double mytimef = MPI_Wtime();
    double times = MPI_Wtime();
    MPI_Gather(vec.data(), num, MPI_INT,
        rbuf.data(), num, MPI_INT, root, MPI_COMM_WORLD);
    double timef = MPI_Wtime();
    if (rank == root) {
        ASSERT_EQ(mybuf, rbuf);
    }
    if (rank == root) {
        std::cout << "my time : " << mytimef - mytimes << std::endl;
        std::cout << "mpi time : " << timef - times << std::endl;
    }
}

TEST(Parallel_Operations_MPI, Test_er_root) {
    int rank, size;
    int root = -5;
    int num = 3;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<int> vec = getRandomArrInt(num, rank);
    std::vector<int> mybuf(size * num);
    if (rank == root) {
        ASSERT_EQ(Gather(vec.data(), num, MPI_INT,
            mybuf.data(), num, MPI_INT, root, MPI_COMM_WORLD), MPI_ERR_ROOT);
    }
}

TEST(Parallel_Operations_MPI, Test_er_count) {
    int rank, size;
    int root = 0;
    int num = 3;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<int> vec = getRandomArrInt(num, rank);
    std::vector<int> mybuf(size * num);
    if (rank == root) {
        ASSERT_EQ(Gather(vec.data(), 2, MPI_INT,
            mybuf.data(), 5, MPI_INT, root, MPI_COMM_WORLD), MPI_ERR_COUNT);
    }
}

int main(int argc, char* argv[]) {
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
