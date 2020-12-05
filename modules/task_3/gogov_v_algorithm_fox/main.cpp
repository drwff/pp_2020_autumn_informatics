// Copyright 2020 Gogov Vladislav
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./algorithm_fox.h"

TEST(Parallel_Multiply_Matrix_MPI, Size_10) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int size = 10;
    bool flag = checkSize(&size);
    if (flag) {
        Matrix matrixA(0), matrixB(0);
        if (procRank == 0) {
            matrixA = createRandomMatrix(size);
            matrixB = createRandomMatrix(size);
        }
        double startTime, endTime;
        startTime = MPI_Wtime();
        Matrix resultParallel = MultiplyMatrixParallel(matrixA, matrixB, size);
        if (procRank == 0) {
            endTime = MPI_Wtime();
            std::cout << "Time parallel: " << endTime - startTime << std::endl;
        }
        if (procRank == 0) {
            startTime = MPI_Wtime();
            std::vector<double> resultSequential = MultiplyMatrixSequential(matrixA, matrixB, size);
            endTime = MPI_Wtime();
            std::cout << "Time sequential: " << endTime - startTime << std::endl;
            ASSERT_TRUE(assertMatrix(resultParallel, resultSequential));
        }
    } else {
        if (procRank == 0) {
            std::cout << "Invalid number of processes." << std::endl;
            ASSERT_FALSE(flag);
        }
    }
}

TEST(Parallel_Multiply_Matrix_MPI, Size_50) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int size = 50;
    bool flag = checkSize(&size);
    if (flag) {
        Matrix matrixA(0), matrixB(0);
        if (procRank == 0) {
            matrixA = createRandomMatrix(size);
            matrixB = createRandomMatrix(size);
        }
        double startTime, endTime;
        startTime = MPI_Wtime();
        Matrix resultParallel = MultiplyMatrixParallel(matrixA, matrixB, size);
        if (procRank == 0) {
            endTime = MPI_Wtime();
            std::cout << "Time parallel: " << endTime - startTime << std::endl;
        }
        if (procRank == 0) {
            startTime = MPI_Wtime();
            std::vector<double> resultSequential = MultiplyMatrixSequential(matrixA, matrixB, size);
            endTime = MPI_Wtime();
            std::cout << "Time sequential: " << endTime - startTime << std::endl;
            ASSERT_TRUE(assertMatrix(resultParallel, resultSequential));
        }
    } else {
        if (procRank == 0) {
            std::cout << "Invalid number of processes." << std::endl;
            ASSERT_FALSE(flag);
        }
    }
}

TEST(Parallel_Multiply_Matrix_MPI, Size_100) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int size = 100;
    bool flag = checkSize(&size);
    if (flag) {
        Matrix matrixA(0), matrixB(0);
        if (procRank == 0) {
            matrixA = createRandomMatrix(size);
            matrixB = createRandomMatrix(size);
        }
        double startTime, endTime;
        startTime = MPI_Wtime();
        Matrix resultParallel = MultiplyMatrixParallel(matrixA, matrixB, size);
        if (procRank == 0) {
            endTime = MPI_Wtime();
            std::cout << "Time parallel: " << endTime - startTime << std::endl;
        }
        if (procRank == 0) {
            startTime = MPI_Wtime();
            std::vector<double> resultSequential = MultiplyMatrixSequential(matrixA, matrixB, size);
            endTime = MPI_Wtime();
            std::cout << "Time sequential: " << endTime - startTime << std::endl;
            ASSERT_TRUE(assertMatrix(resultParallel, resultSequential));
        }
    } else {
        if (procRank == 0) {
            std::cout << "Invalid number of processes." << std::endl;
            ASSERT_FALSE(flag);
        }
    }
}

TEST(Parallel_Multiply_Matrix_MPI, Size_200) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int size = 200;
    bool flag = checkSize(&size);
    if (flag) {
        Matrix matrixA(0), matrixB(0);
        if (procRank == 0) {
            matrixA = createRandomMatrix(size);
            matrixB = createRandomMatrix(size);
        }
        double startTime, endTime;
        startTime = MPI_Wtime();
        Matrix resultParallel = MultiplyMatrixParallel(matrixA, matrixB, size);
        if (procRank == 0) {
            endTime = MPI_Wtime();
            std::cout << "Time parallel: " << endTime - startTime << std::endl;
        }
        if (procRank == 0) {
            startTime = MPI_Wtime();
            std::vector<double> resultSequential = MultiplyMatrixSequential(matrixA, matrixB, size);
            endTime = MPI_Wtime();
            std::cout << "Time sequential: " << endTime - startTime << std::endl;
            ASSERT_TRUE(assertMatrix(resultParallel, resultSequential));
        }
    } else {
        if (procRank == 0) {
            std::cout << "Invalid number of processes." << std::endl;
            ASSERT_FALSE(flag);
        }
    }
}

TEST(Parallel_Multiply_Matrix_MPI, Size_250) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int size = 250;
    bool flag = checkSize(&size);
    if (flag) {
        Matrix matrixA(0), matrixB(0);
        if (procRank == 0) {
            matrixA = createRandomMatrix(size);
            matrixB = createRandomMatrix(size);
        }
        double startTime, endTime;
        startTime = MPI_Wtime();
        Matrix resultParallel = MultiplyMatrixParallel(matrixA, matrixB, size);
        if (procRank == 0) {
            endTime = MPI_Wtime();
            std::cout << "Time parallel: " << endTime - startTime << std::endl;
        }
        if (procRank == 0) {
            startTime = MPI_Wtime();
            std::vector<double> resultSequential = MultiplyMatrixSequential(matrixA, matrixB, size);
            endTime = MPI_Wtime();
            std::cout << "Time sequential: " << endTime - startTime << std::endl;
            ASSERT_TRUE(assertMatrix(resultParallel, resultSequential));
        }
    } else {
        if (procRank == 0) {
            std::cout << "Invalid number of processes." << std::endl;
            ASSERT_FALSE(flag);
        }
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
