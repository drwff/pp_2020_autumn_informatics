// Copyright 2020 Kiseleva Anastasia
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <limits>
#include <iostream>
#include "./zhordan_gauss.h"

const double EPSILON = std::numeric_limits<double>::epsilon()*1e10;

TEST(Parallel_MPI, TEST_2x3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 2;
    double array[6]= { 5, 5 , -5, 7, -3, 10 };
    double *res = new double[2];
    double t0, t1;
    t0 = MPI_Wtime();
    res = zhordan_gauss(array, str);
    t1 = MPI_Wtime();
    if (rank == 0) {
        bool check = Check(array, 2, res, EPSILON);
        ASSERT_TRUE(check);
        std::cout << "paralel_time: " << t1 - t0 << std::endl;
        t0 = MPI_Wtime();
        posled(array, 2);
        t1 = MPI_Wtime();
        std::cout << "posled_time: " << t1 - t0 << std::endl;
    }
}

TEST(Parallel_MPI, TEST_3x4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 3;
    double array[12] = { 3, -9, 15, 15, 5, 8, -10, 2 , 8, -20, 12, -3 };
    double *res = new double[3];
    double t0, t1;
    t0 = MPI_Wtime();
    res = zhordan_gauss(array, str);
    t1 = MPI_Wtime();
    if (rank == 0) {
        bool check = Check(array, 3, res, EPSILON);
        ASSERT_TRUE(check);
        std::cout << "paralel_time: " << t1 - t0 << std::endl;
        t0 = MPI_Wtime();
        posled(array, 3);
        t1 = MPI_Wtime();
        std::cout << "posled_time: " << t1 - t0 << std::endl;
    }
}

TEST(Parallel_MPI, TEST_3x4_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 3;
    double array[12] = { 5, 3, -4, 4, 10, -10 , 1, 1, -3, 5, 2, 4 };
    double *res = new double[3];
    double t0, t1;
    t0 = MPI_Wtime();
    res = zhordan_gauss(array, str);
    t1 = MPI_Wtime();
    if (rank == 0) {
        bool check = Check(array, 3, res, EPSILON);
        ASSERT_TRUE(check);
        std::cout << "paralel_time: " << t1 - t0 << std::endl;
        t0 = MPI_Wtime();
        posled(array, 3);
        t1 = MPI_Wtime();
        std::cout << "posled_time: " << t1 - t0 << std::endl;
    }
}

TEST(Parallel_MPI, TEST_2x3__) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 2;
    double array[6] = { 10, 10 , -10, 12, -8, 15 };
    double *res = new double[2];
    double t0, t1;
    t0 = MPI_Wtime();
    res = zhordan_gauss(array, str);
    t1 = MPI_Wtime();
    if (rank == 0) {
        bool check = Check(array, 2, res, EPSILON);
        ASSERT_TRUE(check);
        std::cout << "paralel_time: " << t1 - t0 << std::endl;
        t0 = MPI_Wtime();
        posled(array, 2);
        t1 = MPI_Wtime();
        std::cout << "posled_time: " << t1 - t0 << std::endl;
    }
}

TEST(Parallel_MPI, TEST_4_x_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 4;
    double array[20] = { 2, 4, 6, 2, 14,
                        -1, 4, 8, 3, 12,
                         8, -6, -1, 4, 5,
                        2 , 8, 9, -11, 10 };
    double *res = new double[4];
    double t0, t1;
    t0 = MPI_Wtime();
    res = zhordan_gauss(array, str);
t1 = MPI_Wtime();
if (rank == 0) {
    bool check = Check(array, 4, res, EPSILON);
    ASSERT_TRUE(check);
    std::cout << "paralel_time: " << t1 - t0 << std::endl;
    t0 = MPI_Wtime();
    posled(array, 4);
    t1 = MPI_Wtime();
    std::cout << "posled_time: " << t1 - t0 << std::endl;
}
}

TEST(Parallel_MPI, TEST_5x6__) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 5;
    double array[30] = { 5, 8, -3, 4, 10, 25,
                        -7, 1, 6, -2, 5, 12,
                         10, -4, 5, 11, -2, 4,
                         -11, 15, -2, 7, 4, -4,
                          7, 10, -5, 5, 15, 18 };
    double *res = new double[5];
    double t0, t1;
    t0 = MPI_Wtime();
    res = zhordan_gauss(array, str);
    t1 = MPI_Wtime();
    if (rank == 0) {
        bool check = Check(array, 5, res, EPSILON);
        ASSERT_TRUE(check);
        std::cout << "paralel_time: " << t1 - t0 << std::endl;
        t0 = MPI_Wtime();
        posled(array, 5);
        t1 = MPI_Wtime();
        std::cout << "posled_time: " << t1 - t0 << std::endl;
    }
}

TEST(Parallel_MPI, TEST_6x7__) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 6;
    double array[42] = { 2, 15, -5, 4, 9, 15, 7,
                          -9, 12, 6, 10, 8, 4, -2,
                          10, 2, -6, 15, 3, -8, 4,
                          15, 1, -9, 11, 8, -6, 21,
                            -9, -5, 21, 30, -11, 5, 14,
                           4, 3, 99, 6, -77, 15, 96 };
    double *res = new double[6];
    double t0, t1;
    t0 = MPI_Wtime();
    res = zhordan_gauss(array, str);
    t1 = MPI_Wtime();
    if (rank == 0) {
        bool check = Check(array, 6, res, EPSILON);
        ASSERT_TRUE(check);
        std::cout << "paralel_time: " << t1 - t0 << std::endl;
        t0 = MPI_Wtime();
        posled(array, 6);
        t1 = MPI_Wtime();
        std::cout << "posled_time: " << t1 - t0 << std::endl;
    }
}

TEST(Parallel_MPI, TEST_10x11__) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 10;
    double array[110] = { 4, 3, 10, 8, 8, 1, 7, 10, 10, 1, 10,
                        8, 1, 4, 10, 2, 7, 8, 5, 4, 5, 2,
                         10, 9, 3, 4, 8, 4, 4, 5, 3, 5, 2,
                          2, 4, 5, 5, 3, 6, 6, 6, 2, 3, 2,
                           9, 10, 10, 8, 1, 3, 2, 9, 9, 8, 6,
                           2, 5, 2, 5, 4, 5, 7, 10, 5, 8, 2,
                           4, 6, 6, 7, 5, 3, 3, 8, 8, 5, 8,
                           10, 2, 3, 1, 9, 2, 5, 7, 6, 3, 7,
                           2, 6, 5, 1, 10, 1, 9, 2, 3, 6, 3,
                           3, 4, 6, 4, 4, 4, 2, 9, 1, 6, 5 };
    double *res = new double[10];
    double t0, t1;
    t0 = MPI_Wtime();
    res = zhordan_gauss(array, str);
    t1 = MPI_Wtime();
    if (rank == 0) {
        bool check = Check(array, 10, res, EPSILON);
        ASSERT_TRUE(check);
        std::cout << "paralel_time: " << t1 - t0 << std::endl;
        t0 = MPI_Wtime();
        posled(array, 10);
        t1 = MPI_Wtime();
        std::cout << "posled_time: " << t1 - t0 << std::endl;
    }
}

TEST(Parallel_MPI, TEST_20x21__) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 20;
    double* array;
    array = new double[str*(str+1)];
    array = matr(array, str);
    double *res = new double[str];
    double t0, t1;
    t0 = MPI_Wtime();
    res = zhordan_gauss(array, str);
    t1 = MPI_Wtime();
    if (rank == 0) {
        bool check = Check(array, str, res, EPSILON);
        ASSERT_TRUE(check);
        std::cout << "paralel_time: " << t1 - t0 << std::endl;
        t0 = MPI_Wtime();
        posled(array, str);
        t1 = MPI_Wtime();
        std::cout << "posled_time: " << t1 - t0 << std::endl;
    }
}

TEST(Parallel_MPI, TEST_220x221__) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 220;
    double* array;
    array = new double[str*(str + 1)];
    array = matr(array, str);
    double *res = new double[str];
    double t0, t1;
    t0 = MPI_Wtime();
    res = zhordan_gauss(array, str);
    t1 = MPI_Wtime();
    if (rank == 0) {
        bool check = Check(array, str, res, EPSILON);
        ASSERT_TRUE(check);
        std::cout << "paralel_time: " << t1 - t0 << std::endl;
        t0 = MPI_Wtime();
        posled(array, str);
        t1 = MPI_Wtime();
        std::cout << "posled_time: " << t1 - t0 << std::endl;
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
