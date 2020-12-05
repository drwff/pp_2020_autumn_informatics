// Copyright 2020 Ekaterina Emelkhovsky
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "./lent_horizontal_only_A.h"


TEST(Parallel_Operations_MPI, Test_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrix_1, matrix_2;

    int s = 100;

    if (rank == 0) {
        matrix_1 = randMatrix(s, s);
        matrix_2 = randMatrix(s, s);
    }

    std::vector<int> parallel_result = getParallelOperations(matrix_1, matrix_2, s);

    if (rank == 0) {
        std::vector<int> easy_result = getSequentialOperations(matrix_1, matrix_2, s);
        ASSERT_EQ(parallel_result, easy_result);
    }
}

TEST(Parallel_Operations_MPI, Test_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrix_1, matrix_2;
    int s = 11;

    if (rank == 0) {
        matrix_1 = randMatrix(s, s);
        matrix_2 = randMatrix(s, s);
    }

    std::vector<int> parallel_result = getParallelOperations(matrix_1, matrix_2, s);

    if (rank == 0) {
        std::vector<int> easy_result = getSequentialOperations(matrix_1, matrix_2, s);
        ASSERT_EQ(parallel_result, easy_result);
    }
}

TEST(Parallel_Operations_MPI, Test_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrix_1, matrix_2;
    int s = 13;

    if (rank == 0) {
        matrix_1 = randMatrix(s, s);
        matrix_2 = randMatrix(s, s);
        ASSERT_NO_THROW(getSequentialOperations(matrix_1, matrix_2, s));
    }
}

TEST(Parallel_Operations_MPI, Test_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrix_1, matrix_2;
    int s = 10;

    if (rank == 0) {
        matrix_1 = randMatrix(s, s);
        matrix_2 = randMatrix(s, s);
    }

    ASSERT_NO_THROW(getParallelOperations(matrix_1, matrix_2, s));
}

TEST(Parallel_Operations_MPI, Test_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrix_1, matrix_2;
    int s = 100;

    if (rank == 0) {
        matrix_1 = randMatrix(s, s);
        matrix_2 = randMatrix(s, s);
    }

    ASSERT_NO_THROW(getParallelOperations(matrix_1, matrix_2, s));
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
