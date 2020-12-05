// Copyright 2020 Skripal Andrey
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./matrixmult.h"


TEST(Parallel_Matrix_Multiplication, 240x240) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size = 240;
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> c;
    std::vector<double> c1;
    if (rank == 0) {
        a = genMatrix(size);
        b = genMatrix(size);
        b = transposition(b, size);
    }
    c = parallelMatrixMult(a, b, size);
    if (rank == 0) {
         c1 = sequentialMatrixMult(a, b, size);
          ASSERT_EQ(c, c1);
    }
}

TEST(Parallel_Matrix_Multiplication, 400x400) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size = 400;
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> c;
    std::vector<double> c1;
    if (rank == 0) {
        a = genMatrix(size);
        b = genMatrix(size);
        b = transposition(b, size);
    }
     c = parallelMatrixMult(a, b, size);
     if (rank == 0) {
          c1 = sequentialMatrixMult(a, b, size);
        ASSERT_EQ(c, c1);
    }
}
TEST(Parallel_Matrix_Multiplication, 64x64) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size = 64;
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> c;
    std::vector<double> c1;
     if (rank == 0) {
          a = genMatrix(size);
          b = genMatrix(size);
          b = transposition(b, size);
     }
     c = parallelMatrixMult(a, b, size);
    if (rank == 0) {
        c1 = sequentialMatrixMult(a, b, size);
        ASSERT_EQ(c, c1);
    }
}
TEST(Parallel_Matrix_Multiplication, 512x512) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size =  512;
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> c;
    std::vector<double> c1;
    if (rank == 0) {
        a = genMatrix(size);
        b = genMatrix(size);
        b = transposition(b, size);
    }
     c = parallelMatrixMult(a, b, size);
     if (rank == 0) {
          c1 = sequentialMatrixMult(a, b, size);
        ASSERT_EQ(c, c1);
    }
}
TEST(Parallel_Matrix_Multiplication, 144x144) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size = 144;
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> c;
    std::vector<double> c1;
     if (rank == 0) {
        a = genMatrix(size);
        b = genMatrix(size);
        b = transposition(b, size);
    }
    c = parallelMatrixMult(a, b, size);
    if (rank == 0) {
        c1 = sequentialMatrixMult(a, b, size);
        ASSERT_EQ(c, c1);
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
