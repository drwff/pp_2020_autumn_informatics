// Copyright 2020 Kirichenko Nikita
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./hypercube.h"

TEST(Parallel_Operations_MPI, Create_Hypercube) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int deg = logOfTwo(size);
    if ((size == degTwo(deg)) && (deg > 0)) {
        MPI_Comm cub = hypercube(deg);
        int realDeg, status;
        int* dim = new int[deg];
        int* per = new int[deg];
        int* coord = new int[deg];
        MPI_Cartdim_get(cub, &realDeg);
        EXPECT_EQ(deg, realDeg);
        MPI_Cart_get(cub, deg, dim, per, coord);
        MPI_Topo_test(cub, &status);
        EXPECT_EQ(status, MPI_CART);
    }
}
TEST(Parallel_Operations_MPI, Check_Periods) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int deg = logOfTwo(size);
    if ((size == degTwo(deg)) && (deg > 0)) {
        MPI_Comm cub = hypercube(deg);
        int realDeg, status;
        int* dim = new int[deg];
        int* per = new int[deg];
        int* coord = new int[deg];
        MPI_Cartdim_get(cub, &realDeg);
        EXPECT_EQ(deg, realDeg);
        MPI_Cart_get(cub, deg, dim, per, coord);
        for (int i = 0; i < deg; i++) {
            EXPECT_EQ(per[i], 1);
        }
        MPI_Topo_test(cub, &status);
        EXPECT_EQ(status, MPI_CART);
    }
}
TEST(Parallel_Operations_MPI, Check_Dimention) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int deg = logOfTwo(size);
    if ((size == degTwo(deg)) && (deg > 0)) {
        MPI_Comm cub = hypercube(deg);
        int realDeg, status;
        int* dim = new int[deg];
        int* per = new int[deg];
        int* coord = new int[deg];
        MPI_Cartdim_get(cub, &realDeg);
        EXPECT_EQ(deg, realDeg);
        MPI_Cart_get(cub, deg, dim, per, coord);
        for (int i = 0; i < deg; i++) {
            EXPECT_EQ(dim[i], 2);
        }
        MPI_Topo_test(cub, &status);
        EXPECT_EQ(status, MPI_CART);
    }
}
TEST(Parallel_Operations_MPI, Err_Create_Hypercube) {
    ASSERT_ANY_THROW(hypercube(-1));
}
TEST(Parallel_Operations_MPI, Trans_Data) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int deg = logOfTwo(size);
    if ((size == degTwo(deg)) && (deg > 0)) {
        MPI_Comm cub = hypercube(deg);
        int tmp = -1, a, b;
        for (int i = 0; i < deg; i++) {
            MPI_Cart_shift(cub, i, 1, &a, &b);
            if ((a == MPI_PROC_NULL) || (b == MPI_PROC_NULL)) {
                tmp = -2;
            }
        }
        if (rank == 0) {
            tmp = -2;
        }
        for (int i = 0; i < size; i++)
            MPI_Bcast(&tmp, 1, MPI_INT, 0, cub);
        ASSERT_EQ(tmp, -2);
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
