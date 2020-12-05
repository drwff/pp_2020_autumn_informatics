// Copyright 2020 Loganov Andrei
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <ctime>
#include <numeric>
#include "./smoothing.h"
TEST(TEST_PARALEL_MPI, TEST1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 3;
    int cols = 4;
    int radius = 1;
    int** test;
    test = new int* [rows];
    for (int i = 0; i < rows; i++) {
        test[i] = new int[cols];
    }
    test[0][0] = 236;
    test[0][1] = 127;
    test[0][2] = 141;
    test[0][3] = 4;
    test[1][0] = 21;
    test[1][1] = 108;
    test[1][2] = 114;
    test[1][3] = 59;
    test[2][0] = 157;
    test[2][1] = 53;
    test[2][2] = 229;
    test[2][3] = 203;
    int** globaltest;
    globaltest = new int* [rows];
    for (int i = 0; i < rows; i++) {
        globaltest[i] = new int[cols];
    }
    globaltest = ParallelSmoothing(test, cols, rows, radius);
    bool check = true;
    if (rank == 0) {
        int** localtest;
        localtest = new int* [rows];
        for (int i = 0; i < rows; i++) {
             localtest[i] = new int[cols];
        }
        localtest[0][0] = 128;
        localtest[0][1] = 128;
        localtest[0][2] = 72;
        localtest[0][3] = 72;
        localtest[1][0] = 128;
        localtest[1][1] = 128;
        localtest[1][2] = 116;
        localtest[1][3] = 116;
        localtest[2][0] = 89;
        localtest[2][1] = 125;
        localtest[2][2] = 141;
        localtest[2][3] = 144;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (localtest[i][j] != globaltest[i][j]) {
                    check = false;
                }
            }
        }
        arrdel(localtest, rows);
    }
    arrdel(globaltest, rows);
    arrdel(test, rows);
    if (rank == 0) {
        ASSERT_EQ(check, true);
    }
}

TEST(TEST_PARALEL_MPI, TEST2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 4;
    int cols = 4;
    int radius = 1;
    int** test;
    test = new int* [rows];
    for (int i = 0; i < rows; i++) {
        test[i] = new int[cols];
    }
    test[0][0] = 88;
    test[0][1] = 13;
    test[0][2] = 198;
    test[0][3] = 227;
    test[1][0] = 166;
    test[1][1] = 227;
    test[1][2] = 148;
    test[1][3] = 78;
    test[2][0] = 243;
    test[2][1] = 204;
    test[2][2] = 32;
    test[2][3] = 140;
    test[3][0] = 173;
    test[3][1] = 26;
    test[3][2] = 81;
    test[3][3] = 43;
    int** globaltest;
    globaltest = new int* [rows];
    for (int i = 0; i < rows; i++) {
        globaltest[i] = new int[cols];
    }
    globaltest = ParallelSmoothing(test, cols, rows, radius);
    bool check = true;
    if (rank == 0) {
        int** localtest;
        localtest = new int* [rows];
        for (int i = 0; i < rows; i++) {
             localtest[i] = new int[cols];
        }
        localtest[0][0] = 120;
        localtest[0][1] = 120;
        localtest[0][2] = 120;
        localtest[0][3] = 152;
        localtest[1][0] = 128;
        localtest[1][1] = 128;
        localtest[1][2] = 120;
        localtest[1][3] = 129;
        localtest[2][0] = 134;
        localtest[2][1] = 134;
        localtest[2][2] = 126;
        localtest[2][3] = 90;
        localtest[3][0] = 134;
        localtest[3][1] = 134;
        localtest[3][2] = 115;
        localtest[3][3] = 86;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (localtest[i][j] != globaltest[i][j]) {
                    check = false;
                }
            }
        }
        arrdel(localtest, rows);
    }
    arrdel(globaltest, rows);
    arrdel(test, rows);
    if (rank == 0) {
        ASSERT_EQ(check, true);
    }
}
TEST(TEST_PARALEL_MPI, TEST3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 4;
    int cols = 5;
    int radius = 1;
    int** test;
    test = new int* [rows];
    for (int i = 0; i < rows; i++) {
        test[i] = new int[cols];
    }
    test[0][0] = 136;
    test[0][1] = 11;
    test[0][2] = 213;
    test[0][3] = 238;
    test[0][4] = 7;
    test[1][0] = 183;
    test[1][1] = 216;
    test[1][2] = 211;
    test[1][3] = 151;
    test[1][4] = 97;
    test[2][0] = 178;
    test[2][1] = 142;
    test[2][2] = 238;
    test[2][3] = 71;
    test[2][4] = 80;
    test[3][0] = 163;
    test[3][1] = 122;
    test[3][2] = 107;
    test[3][3] = 126;
    test[3][4] = 199;
    int** globaltest;
    globaltest = new int* [rows];
    for (int i = 0; i < rows; i++) {
        globaltest[i] = new int[cols];
    }
    globaltest = ParallelSmoothing(test, cols, rows, radius);
    if (rank == 0) {
        int** localtest;
        localtest = new int* [rows];
        for (int i = 0; i < rows; i++) {
             localtest[i] = new int[cols];
        }
        localtest[0][0] = 113;
        localtest[0][1] = 113;
        localtest[0][2] = 124;
        localtest[0][3] = 122;
        localtest[0][4] = 122;
        localtest[1][0] = 113;
        localtest[1][1] = 124;
        localtest[1][2] = 124;
        localtest[1][3] = 122;
        localtest[1][4] = 122;
        localtest[2][0] = 169;
        localtest[2][1] = 172;
        localtest[2][2] = 154;
        localtest[2][3] = 154;
        localtest[2][4] = 135;
        localtest[3][0] = 150;
        localtest[3][1] = 172;
        localtest[3][2] = 154;
        localtest[3][3] = 154;
        localtest[3][4] = 135;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                ASSERT_EQ(localtest[i][j], globaltest[i][j]);
            }
        }
        arrdel(localtest, rows);
    }
    arrdel(globaltest, rows);
    arrdel(test, rows);
}
TEST(TEST_PARALEL_MPI, TEST4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 5;
    int cols = 5;
    int radius = 1;
    int** test;
    test = new int* [rows];
    for (int i = 0; i < rows; i++) {
        test[i] = new int[cols];
    }
    test[0][0] = 150;
    test[0][1] = 151;
    test[0][2] = 50;
    test[0][3] = 71;
    test[0][4] = 118;
    test[1][0] = 76;
    test[1][1] = 211;
    test[1][2] = 112;
    test[1][3] = 191;
    test[1][4] = 129;
    test[2][0] = 28;
    test[2][1] = 235;
    test[2][2] = 91;
    test[2][3] = 67;
    test[2][4] = 167;
    test[3][0] = 31;
    test[3][1] = 101;
    test[3][2] = 129;
    test[3][3] = 236;
    test[3][4] = 242;
    test[4][0] = 149;
    test[4][1] = 109;
    test[4][2] = 188;
    test[4][3] = 74;
    test[4][4] = 144;
    int** globaltest;
    globaltest = new int* [rows];
    for (int i = 0; i < rows; i++) {
        globaltest[i] = new int[cols];
    }
    globaltest = ParallelSmoothing(test, cols, rows, radius);
    if (rank == 0) {
        int** localtest;
        localtest = new int* [rows];
        for (int i = 0; i < rows; i++) {
             localtest[i] = new int[cols];
        }
        localtest[0][0] = 143;
        localtest[0][1] = 130;
        localtest[0][2] = 130;
        localtest[0][3] = 120;
        localtest[0][4] = 131;
        localtest[1][0] = 131;
        localtest[1][1] = 131;
        localtest[1][2] = 142;
        localtest[1][3] = 120;
        localtest[1][4] = 129;
        localtest[2][0] = 131;
        localtest[2][1] = 131;
        localtest[2][2] = 151;
        localtest[2][3] = 154;
        localtest[2][4] = 154;
        localtest[3][0] = 131;
        localtest[3][1] = 131;
        localtest[3][2] = 151;
        localtest[3][3] = 154;
        localtest[3][4] = 154;
        localtest[4][0] = 90;
        localtest[4][1] = 109;
        localtest[4][2] = 155;
        localtest[4][3] = 158;
        localtest[4][4] = 158;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                ASSERT_EQ(localtest[i][j], globaltest[i][j]);
            }
        }
        arrdel(localtest, rows);
    }
    arrdel(globaltest, rows);
    arrdel(test, rows);
}
TEST(TEST_PARALEL_MPI, TEST5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 2;
    int cols = 5;
    int radius = 1;
    int** test;
    test = new int* [rows];
    for (int i = 0; i < rows; i++) {
        test[i] = new int[cols];
    }
    test[0][0] = 157;
    test[0][1] = 72;
    test[0][2] = 210;
    test[0][3] = 130;
    test[0][4] = 119;
    test[1][0] = 62;
    test[1][1] = 208;
    test[1][2] = 35;
    test[1][3] = 253;
    test[1][4] = 186;
    int** globaltest;
    globaltest = new int* [rows];
    for (int i = 0; i < rows; i++) {
        globaltest[i] = new int[cols];
    }
    globaltest = ParallelSmoothing(test, cols, rows, radius);
    if (rank == 0) {
        int** localtest;
        localtest = new int* [rows];
        for (int i = 0; i < rows; i++) {
             localtest[i] = new int[cols];
        }
        localtest[0][0] = 135;
        localtest[0][1] = 122;
        localtest[0][2] = 144;
        localtest[0][3] = 144;
        localtest[0][4] = 186;
        localtest[1][0] = 135;
        localtest[1][1] = 122;
        localtest[1][2] = 144;
        localtest[1][3] = 144;
        localtest[1][4] = 186;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                ASSERT_EQ(localtest[i][j], globaltest[i][j]);
            }
        }
        arrdel(localtest, rows);
    }
    arrdel(globaltest, rows);
    arrdel(test, rows);
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
