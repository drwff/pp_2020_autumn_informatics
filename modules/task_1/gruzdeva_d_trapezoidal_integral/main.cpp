// Copyright 2020 Gruzdeva Diana
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cmath>
#include "./trapezoidal_integral.h"

std::function<double(double)> polinom = polinomFunction;
std::function<double(double)> composite = compositeFunction;


TEST(Parallel_Operations_MPI, POLINOM_FROM_0_TO_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double parallel_result = getParallelIntegral(0, 1, 10000, polinom);

    if (rank == 0) {
        double sequential_result = getSequentialIntegral(0, 1, 10000, polinom);
        ASSERT_LT(std::fabs(parallel_result - sequential_result), 1.0e-8);
    }
}

TEST(Parallel_Operations_MPI, POLINOM_FROM_0_TO_10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double parallel_result = getParallelIntegral(0, 10, 10000, polinom);

    if (rank == 0) {
        double sequential_result = getSequentialIntegral(0, 10, 10000, polinom);
        ASSERT_LT(std::fabs(parallel_result - sequential_result), 1.0e-4);
    }
}

TEST(Parallel_Operations_MPI, POLINOM_FROM_0_TO_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double parallel_result = getParallelIntegral(0, 1000, 10000, polinom);

    if (rank == 0) {
        double sequential_result = getSequentialIntegral(0, 1000, 10000, polinom);
        ASSERT_LT(std::fabs(parallel_result - sequential_result), 1.0e+4);
    }
}

TEST(Parallel_Operations_MPI, COMPOSITE_FUNCTION_FROM_0_TO_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double parallel_result = getParallelIntegral(0, 1, 10000, composite);

    if (rank == 0) {
        double sequential_result = getSequentialIntegral(0, 1, 10000, composite);
        ASSERT_LT(std::fabs(parallel_result - sequential_result), 1.0e-8);
    }
}

TEST(Parallel_Operations_MPI, COMPOSITE_FUNCTION_FROM_0_TO_0_001) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double parallel_result = getParallelIntegral(0, 0.001, 10000, composite);

    if (rank == 0) {
        double sequential_result = getSequentialIntegral(0, 0.001, 10000, composite);
        ASSERT_LT(std::fabs(parallel_result - sequential_result), 1.0e-17);
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
