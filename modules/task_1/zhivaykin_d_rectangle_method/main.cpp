// Copyright 2020 Zhivaykin Daniil
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include "./rectangle.h"

std::function<double(double)> funct = func;

TEST(Parallel_Operations_MPI, RET_50) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double p_res = parall_int(funct, 0, 2, 50);

    if (rank == 0) {
        double d_res = def_int(funct, 0, 2, 50);
        ASSERT_LT(std::fabs(p_res - d_res), std::numeric_limits<double>::epsilon() * 100);
    }
}

TEST(Parallel_Operations_MPI, RET_500) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double p_res = parall_int(funct, 0, 2, 500);

    if (rank == 0) {
        double d_res = def_int(funct, 0, 2, 500);
        ASSERT_LT(std::fabs(p_res - d_res), std::numeric_limits<double>::epsilon() * 100);
    }
}

TEST(Parallel_Operations_MPI, RET_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double p_res = parall_int(funct, 0, 2, 1000);

    if (rank == 0) {
        double d_res = def_int(funct, 0, 2, 1000);
        ASSERT_LT(std::fabs(p_res - d_res), std::numeric_limits<double>::epsilon() * 100);
    }
}

TEST(Parallel_Operations_MPI, RET_5000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double p_res = parall_int(funct, 0, 2, 5000);

    if (rank == 0) {
        double d_res = def_int(funct, 0, 2, 5000);
        ASSERT_LT(std::fabs(p_res - d_res), std::numeric_limits<double>::epsilon() * 100);
    }
}

TEST(Parallel_Operations_MPI, RET_10000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double p_res = parall_int(funct, 0, 1, 10000);

    if (rank == 0) {
        double d_res = def_int(funct, 0, 1, 10000);
        ASSERT_LT(std::fabs(p_res - d_res), std::numeric_limits<double>::epsilon() * 100);
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
