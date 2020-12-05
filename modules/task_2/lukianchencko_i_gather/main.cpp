// Copyright 2020 Luckyanchencko Ivan
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <random>
#include <vector>
#include "./gather.h"

TEST(Paral, Test_gather_DOUBLE) {
    int ProcRank, ProcNum, rcount = 5, root = 1;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int n = rcount*ProcNum;
    std::vector<double> rbuf(rcount);
    std::vector<double> res(n);
    std::vector<double> res1(n);
    for (int i = 0; i < rcount; i++) {
        rbuf[i] = (ProcRank+1)*i;
    }
    Gather(rbuf.data(), rcount, MPI_DOUBLE, res1.data(), rcount, MPI_DOUBLE, root, MPI_COMM_WORLD);
    MPI_Gather(rbuf.data(), rcount, MPI_DOUBLE, res.data(), rcount, MPI_DOUBLE, root, MPI_COMM_WORLD);
    if (ProcRank == root) {
        for (int i = 0; i < n; i++) {
        //  std::cout << res[i] << std::endl;
        ASSERT_EQ(res[i], res1[i]);
    }
    }
}
TEST(Paral, Test_gather_INT) {
    int ProcRank, ProcNum, rcount = 3, root = 1;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int n = rcount*ProcNum;
    std::vector<int> rbuf(rcount);
    std::vector<int> res(n);
    std::vector<int> res1(n);
    for (int i = 0; i < rcount; i++) {
        rbuf[i] = (ProcRank+1)*i;
    }
    MPI_Gather(rbuf.data(), rcount, MPI_INT, res.data(), rcount, MPI_INT, root, MPI_COMM_WORLD);
    Gather(rbuf.data(), rcount, MPI_INT, res1.data(), rcount, MPI_INT, root, MPI_COMM_WORLD);
    if (ProcRank == root) {
        for (int i = 0; i < n; i++) {
        // std::cout<<res[i]<<std::endl;
        ASSERT_EQ(res[i], res1[i]);
    }
    }
}
TEST(Paral, Test_gather_FLOAT) {
    int ProcRank, ProcNum, rcount = 15, root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int n = rcount*ProcNum;
    std::vector<float> rbuf(rcount);
    std::vector<float> res(n);
    std::vector<float> res1(n);
    for (int i = 0; i < rcount; i++) {
        rbuf[i] = static_cast<float>((ProcRank+1)*i);
    }
    MPI_Gather(&rbuf[0], rcount, MPI_FLOAT, &res[0], rcount, MPI_FLOAT, root, MPI_COMM_WORLD);
    Gather(&rbuf[0], rcount, MPI_FLOAT, &res1[0], rcount, MPI_FLOAT, root, MPI_COMM_WORLD);
    if (ProcRank == root) {
        for (int i = 0; i < n; i++) {
        // std::cout<<res[i]<<std::endl;
        ASSERT_EQ(res[i], res1[i]);
    }
    }
}
TEST(Paral, Test_gather_COUNT) {
    int ProcRank, ProcNum, rcount = 25, root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int n = rcount*ProcNum;
    std::vector<double> rbuf(rcount);
    std::vector<double> res(n);
    std::vector<double> res1(n);
    for (int i = 0; i < rcount; i++) {
        rbuf[i] = (ProcRank+1)*i;
    }
    MPI_Gather(&rbuf[0], rcount, MPI_DOUBLE, &res[0], rcount, MPI_DOUBLE, root, MPI_COMM_WORLD);
    Gather(&rbuf[0], rcount, MPI_DOUBLE, &res1[0], rcount, MPI_DOUBLE, root, MPI_COMM_WORLD);
    if (ProcRank == root) {
        for (int i = 0; i < n; i++) {
        // std::cout<<res1[i]<<std::endl;
        ASSERT_EQ(res[i], res1[i]);
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
