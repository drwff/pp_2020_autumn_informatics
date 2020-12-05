// Copyright 2020 Nadir mohammed
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include<mpi.h>
#include<vector>
#include "./Methodgauss.h"


    TEST(GAUSS_PARALLEL_MPI, 2x3) {
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        int row = 2, col = 3;
        double* array1{ new double[row * col]{ 1, 2, 3, 4, 5, 6 } };
        double* sub_solution1 = new double[row];
        methodGaussParallel(array1, sub_solution1, row, col);



        //**************************************
        std::vector<std::vector<double>> array2{ {1, 2, 3} , {4, 5, 6} };
        double* sub_solution2 = new double[row];
        methodGauss(array2, sub_solution2, row);

        if (rank == 0) {
                for (int i = 0; i < row; i++) {
                    ASSERT_NEAR(sub_solution1[i], sub_solution2[i], 0.00001);
                }
            }
        delete[] array1;
        delete[] sub_solution1;
    }
    TEST(GAUSS_PARALLEL_MPI, 3x4) {
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        int row = 3, col = 4;
        double* array1{ new double[row * col]{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } };
        double* sub_solution1 = new double[row];
        methodGaussParallel(array1, sub_solution1, row, col);



        //**************************************
        std::vector<std::vector<double>> array2{ {1, 2, 3, 4} , {5, 6, 7, 8} , {9, 10, 11, 12} };
        double* sub_solution2 = new double[row];
        methodGauss(array2, sub_solution2, row);

        if (rank == 0) {
            for (int i = 0; i < row; i++) {
                for (int i = 0; i < row; i++) {
                    ASSERT_NEAR(sub_solution1[i], sub_solution2[i], 0.00001);
                }
            }
            delete[] array1;
            delete[] sub_solution1;
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
