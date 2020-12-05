// Copyright 2020 Bakaeva Maria
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./gauss_horizontal_scheme.h"

TEST(GaussHorizontalScheme, testWorkWithMatrix3x4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int rows = 3;
    int cols = 4;
    double* matrix = new double[rows * cols];
    double* resParallel = new double[rows];
    matrix[0] = 8; matrix[1] = 7; matrix[2] = 3; matrix[3] = 18;
    matrix[4] = -7; matrix[5] = -4; matrix[6] = -4; matrix[7] = -11;
    matrix[8] = -6; matrix[9] = 5; matrix[10] = -4; matrix[11] = -15;

    getParallelGauss(matrix, resParallel, cols, rows);

    if (rank == 0) {
        double* resLinear = new double[rows];
        getSequentialGauss(matrix, resLinear, rows);
        for (int i = 0; i < rows; i++) {
            ASSERT_NEAR(resLinear[i], resParallel[i], 0.00001);
        }

        delete[] resLinear;
    }

    delete[] matrix;
    delete[] resParallel;
}

TEST(GaussHorizontalScheme, testWorkWithMatrix4x5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int rows = 4;
    int cols = 5;
    double* matrix = new double[rows * cols];
    double* resParallel = new double[rows];
    matrix[0] = 1; matrix[1] = 1; matrix[2] = 2; matrix[3] = 3; matrix[4] = -13;
    matrix[5] = 1; matrix[6] = -2; matrix[7] = 3; matrix[8] = -1; matrix[9] = 14;
    matrix[10] = 3; matrix[11] = -1; matrix[12] = -1; matrix[13] = -2; matrix[14] = 7;
    matrix[15] = 2; matrix[16] = 3; matrix[17] = -1; matrix[18] = -1; matrix[19] = 9;

    getParallelGauss(matrix, resParallel, cols, rows);

    if (rank == 0) {
        double* resLinear = new double[rows];
        getSequentialGauss(matrix, resLinear, rows);
        for (int i = 0; i < rows; i++) {
            ASSERT_NEAR(resLinear[i], resParallel[i], 0.00001);
        }
        delete[] resLinear;
    }

    delete[] matrix;
    delete[] resParallel;
}

TEST(GaussHorizontalScheme, testWorkWithMatrix2x3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int rows = 2;
    int cols = 3;
    double* matrix = new double[rows * cols];
    double* resParallel = new double[rows];
    matrix[0] = 1; matrix[1] = 2; matrix[2] = 5;
    matrix[3] = 3; matrix[4] = 3000; matrix[5] = 7;

    getParallelGauss(matrix, resParallel, cols, rows);

    if (rank == 0) {
        double* resLinear = new double[rows];
        getSequentialGauss(matrix, resLinear, rows);
        for (int i = 0; i < rows; i++) {
            ASSERT_NEAR(resLinear[i], resParallel[i], 0.00001);
        }
        delete[] resLinear;
    }

    delete[] matrix;
    delete[] resParallel;
}

TEST(GaussHorizontalScheme, testWorkWithMatrix5x6) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int rows = 5;
    int cols = 6;
    double* matrix = new double[rows * cols];
    double* resParallel = new double[rows];
    matrix[0] = 123; matrix[1] = 3; matrix[2] = 8; matrix[3] = 0; matrix[4] = 2; matrix[5] = 6;
    matrix[6] = 8; matrix[7] = 2; matrix[8] = 6; matrix[9] = 9; matrix[10] = -11; matrix[11] = 2;
    matrix[12] = -36; matrix[13] = 23; matrix[14] = 5; matrix[15] = 1; matrix[16] = 7; matrix[17] = 71;
    matrix[18] = 5; matrix[19] = -8; matrix[20] = 1; matrix[21] = 6; matrix[22] = 6; matrix[23] = 4;
    matrix[24] = 25; matrix[25] = 8; matrix[26] = 4; matrix[27] = 6; matrix[28] = 5; matrix[29] = 0;

    getParallelGauss(matrix, resParallel, cols, rows);

    if (rank == 0) {
        double* resLinear = new double[rows];
        getSequentialGauss(matrix, resLinear, rows);
        for (int i = 0; i < rows; i++) {
            ASSERT_NEAR(resLinear[i], resParallel[i], 0.00001);
        }
        delete[] resLinear;
    }

    delete[] matrix;
    delete[] resParallel;
}

TEST(GaussHorizontalScheme, testWorkWithMatrix6x7) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int rows = 6;
    int cols = 7;
    double* matrix = new double[rows * cols];
    double* resParallel = new double[rows];
    matrix[0] = 54; matrix[1] = 8; matrix[2] = 57; matrix[3] = -7; matrix[4] = -37; matrix[5] = -7;
    matrix[6] = 6; matrix[7] = 7; matrix[8] = 6; matrix[9] = 67; matrix[10] = -5; matrix[11] = 24;
    matrix[12] = 74; matrix[13] = 2; matrix[14] = 5; matrix[15] = 24; matrix[16] = 48; matrix[17] = 6;
    matrix[18] = 25; matrix[19] = 32; matrix[20] = 2; matrix[21] = 46; matrix[22] = 12; matrix[23] = -6;
    matrix[24] = 15; matrix[25] = 0; matrix[26] = 25; matrix[27] = 45; matrix[28] = -6; matrix[29] = 5;
    matrix[30] = -4; matrix[31] = 4; matrix[32] = 8; matrix[33] = 9; matrix[34] = 1; matrix[35] = 44;
    matrix[36] = 4; matrix[37] = 3; matrix[38] = 2; matrix[39] = 1; matrix[40] = 5; matrix[41] = 6;

    getParallelGauss(matrix, resParallel, cols, rows);

    if (rank == 0) {
        double* resLinear = new double[rows];
        getSequentialGauss(matrix, resLinear, rows);
        for (int i = 0; i < rows; i++) {
            ASSERT_NEAR(resLinear[i], resParallel[i], 0.00001);
        }
        delete[] resLinear;
    }

    delete[] matrix;
    delete[] resParallel;
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
