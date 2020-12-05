// Copyright 2020 Gruzdeva Diana
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "./mat_mult_horiz_only.h"

TEST(Parallel_Operations_MPI, MATRICES_10x10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> a;
    std::vector<double> b;

    int aR = 10, aC = 10, bC = 10;

    if (rank == 0) {
        a = getRandomMatrix(aR, aC, time(0));
        b = getRandomMatrix(aC, bC, time(0) + 1);
    }

    double start, end;
    if (rank == 0) {
        start = MPI_Wtime();
    }
    std::vector<double> parallel_result = getParallelMultiplication(a, b, aR, aC, bC);

    if (rank == 0) {
        end = MPI_Wtime();
        std::cout << "Time for parallel is " << end - start << std::endl;
        start = MPI_Wtime();
        std::vector<double> sequential_result = getSequentialMultiplication(a, b, aR, aC, bC);
        end = MPI_Wtime();
        std::cout << "Time for sequential is " << end - start << std::endl;
        ASSERT_EQ(sequential_result, parallel_result);
    }
}

TEST(Parallel_Operations_MPI, MATRICES_25x25) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> a;
    std::vector<double> b;

    int aR = 25, aC = 25, bC = 25;

    if (rank == 0) {
        a = getRandomMatrix(aR, aC, time(0));
        b = getRandomMatrix(aC, bC, time(0) + 1);
    }

    double start, end;
    if (rank == 0) {
        start = MPI_Wtime();
    }
    std::vector<double> parallel_result = getParallelMultiplication(a, b, aR, aC, bC);

    if (rank == 0) {
        end = MPI_Wtime();
        std::cout << "Time for parallel is " << end - start << std::endl;
        start = MPI_Wtime();
        std::vector<double> sequential_result = getSequentialMultiplication(a, b, aR, aC, bC);
        end = MPI_Wtime();
        std::cout << "Time for sequential is " << end - start << std::endl;
        ASSERT_EQ(sequential_result, parallel_result);
    }
}

TEST(Parallel_Operations_MPI, MATRICES_75x75) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> a;
    std::vector<double> b;

    int aR = 75, aC = 75, bC = 75;

    if (rank == 0) {
        a = getRandomMatrix(aR, aC, time(0));
        b = getRandomMatrix(aC, bC, time(0) + 1);
    }

    double start, end;
    if (rank == 0) {
        start = MPI_Wtime();
    }
    std::vector<double> parallel_result = getParallelMultiplication(a, b, aR, aC, bC);

    if (rank == 0) {
        end = MPI_Wtime();
        std::cout << "Time for parallel is " << end - start << std::endl;
        start = MPI_Wtime();
        std::vector<double> sequential_result = getSequentialMultiplication(a, b, aR, aC, bC);
        end = MPI_Wtime();
        std::cout << "Time for sequential is " << end - start << std::endl;
        ASSERT_EQ(sequential_result, parallel_result);
    }
}

TEST(Parallel_Operations_MPI, MATRICES_100x100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> a;
    std::vector<double> b;

    int aR = 100, aC = 100, bC = 100;

    if (rank == 0) {
        a = getRandomMatrix(aR, aC, time(0));
        b = getRandomMatrix(aC, bC, time(0) + 1);
    }

    double start, end;
    if (rank == 0) {
        start = MPI_Wtime();
    }
    std::vector<double> parallel_result = getParallelMultiplication(a, b, aR, aC, bC);

    if (rank == 0) {
        end = MPI_Wtime();
        std::cout << "Time for parallel is " << end - start << std::endl;
        start = MPI_Wtime();
        std::vector<double> sequential_result = getSequentialMultiplication(a, b, aR, aC, bC);
        end = MPI_Wtime();
        std::cout << "Time for sequential is " << end - start << std::endl;
        ASSERT_EQ(sequential_result, parallel_result);
    }
}


TEST(Parallel_Operations_MPI, MATRICES_240x240) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> a;
    std::vector<double> b;

    int aR = 240, aC = 240, bC = 240;

    if (rank == 0) {
        a = getRandomMatrix(aR, aC, time(0));
        b = getRandomMatrix(aC, bC, time(0) + 1);
    }

    double start, end;
    if (rank == 0) {
        start = MPI_Wtime();
    }
    std::vector<double> parallel_result = getParallelMultiplication(a, b, aR, aC, bC);

    if (rank == 0) {
        end = MPI_Wtime();
        std::cout << "Time for parallel is " << end - start << std::endl;
        start = MPI_Wtime();
        std::vector<double> sequential_result = getSequentialMultiplication(a, b, aR, aC, bC);
        end = MPI_Wtime();
        std::cout << "Time for sequential is " << end - start << std::endl;
        ASSERT_EQ(sequential_result, parallel_result);
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
