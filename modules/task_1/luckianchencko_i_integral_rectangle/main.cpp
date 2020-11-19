// Copyright 2020 Luckianchencko Ivan
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include<mpi.h>
#include<cmath>
#include "./integral.h"

TEST(Paral, Test_func1_1) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double paral_answer = get_Paral_Integral(func1, 2, 5, 10);
    double answer = 0.5363761633138187;
    if (ProcRank == 0) {
        ASSERT_LE(paral_answer, answer);
    }
}
TEST(Paral, Test_func1_2) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double paral_answer = get_Paral_Integral(func1, 1, 10, 100);
    double answer = 1.474004572610699;
    if (ProcRank == 0) {
        ASSERT_LE(paral_answer, answer);
    }
}
TEST(Paral, Test_func1_3) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double paral_answer = get_Paral_Integral(func1, 2, 15, 10000);
    double answer = 1.271003664144179;
    if (ProcRank == 0) {
        ASSERT_LE(paral_answer, answer);
    }
}
TEST(Paral, Test_func2_1) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double paral_answer = get_Paral_Integral(func2, 2, 5, 10);
    double answer = 2.495135691915521;
    if (ProcRank == 0) {
        ASSERT_LE(paral_answer, answer);
    }
}
TEST(Paral, Test_func2_2) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double paral_answer = get_Paral_Integral(func2, 1, 15, 100);
    double answer = 1246.917915938373;
    if (ProcRank == 0) {
        ASSERT_LE(paral_answer, answer);
    }
}
TEST(Paral, Test_func2_3) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double paral_answer = get_Paral_Integral(func2, 2, 20, 10000);
    double answer = 72228.8621069474;
    if (ProcRank == 0) {
        ASSERT_LE(paral_answer, answer);
    }
}
TEST(Paral, Test_func3_1) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double paral_answer = get_Paral_Integral(func3, 2, 5, 10);
    double answer = 0.02676495780795789;
    if (ProcRank == 0) {
        ASSERT_LE(paral_answer, answer);
    }
}
TEST(Paral, Test_func3_2) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double paral_answer = get_Paral_Integral(func3, 1, 12, 100);
    double answer = 0.1897329721344028;
    if (ProcRank == 0) {
        ASSERT_LE(paral_answer, answer);
    }
}
TEST(Paral, Test_func3_3) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double paral_answer = get_Paral_Integral(func3, 2, 15, 10000);
    double answer = 0.02456527744571311;
    if (ProcRank == 0) {
        ASSERT_LE(paral_answer, answer);
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
