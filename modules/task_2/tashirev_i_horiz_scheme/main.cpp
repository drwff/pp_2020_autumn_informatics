// Copyright 2020 Tashirev Ivan
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>

#include "../../../modules/task_2/tashirev_i_horiz_scheme/RibbonHorizontalPattern.h"

TEST(Parallel_Operations_MPI, Test_Matrix4_3) {
    const int rows = 4;
    const int columns = 3;
    std::vector<int> matr;
    std::vector<int> vec;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matr = getMatrix(rows, columns);
        vec = getVector(columns);
    }
    std::vector<int> multiply = multMatrixParallel(matr, rows, columns, vec, columns);
    if (rank == 0) {
        std::vector<int> multiplyNotParall = multMatrixSeq(matr, rows, columns, vec, columns);
            ASSERT_EQ(multiplyNotParall, multiply);
    }
}

TEST(Parallel_Operations_MPI, Test_Matrix5_3) {
    const int rows = 5;
    const int columns = 3;
    std::vector<int> matr;
    std::vector<int> vec;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matr = getMatrix(rows, columns);
        vec = getVector(columns);
    }
    std::vector<int> multiply = multMatrixParallel(matr, rows, columns, vec, columns);
    if (rank == 0) {
        std::vector<int> multiplyNotParall = multMatrixSeq(matr, rows, columns, vec, columns);
        ASSERT_EQ(multiplyNotParall, multiply);
    }
}

TEST(Parallel_Operations_MPI, Test_Matrix10_10) {
    const int rows = 10;
    const int columns = 10;
    std::vector<int> matr;
    std::vector<int> vec;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matr = getMatrix(rows, columns);
        vec = getVector(columns);
    }
    std::vector<int> multiply = multMatrixParallel(matr, rows, columns, vec, columns);
    if (rank == 0) {
        std::vector<int> multiplyNotParall = multMatrixSeq(matr, rows, columns, vec, columns);
        ASSERT_EQ(multiplyNotParall, multiply);
    }
}

TEST(Parallel_Operations_MPI, Test_Matrix10_5) {
    const int rows = 10;
    const int columns = 5;
    std::vector<int> matr;
    std::vector<int> vec;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matr = getMatrix(rows, columns);
        vec = getVector(columns);
    }
    std::vector<int> multiply = multMatrixParallel(matr, rows, columns, vec, columns);
    if (rank == 0) {
        std::vector<int> multiplyNotParall = multMatrixSeq(matr, rows, columns, vec, columns);
        ASSERT_EQ(multiplyNotParall, multiply);
    }
}

TEST(Parallel_Operations_MPI, Test_Matrix20_5) {
    const int rows = 20;
    const int columns = 5;
    std::vector<int> matr;
    std::vector<int> vec;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matr = getMatrix(rows, columns);
        vec = getVector(columns);
    }
    std::vector<int> multiply = multMatrixParallel(matr, rows, columns, vec, columns);
    if (rank == 0) {
        std::vector<int> multiplyNotParall = multMatrixSeq(matr, rows, columns, vec, columns);
        ASSERT_EQ(multiplyNotParall, multiply);
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
