// Copyright 2020 Volkova Anastasia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <random>
#include <vector>
#include "./rectangle_method_mpi.h"

TEST(Integral, Test1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double integralStart = MPI_Wtime();
    double integral = integralFunction(function1, 0, 17, -1, 2, 4, 10, 10, 10, 10);
    double integralEnd = MPI_Wtime();
    double parallStart = MPI_Wtime();
    double parall = getCalculatedIntegral(function1, 0, 17, -1, 2, 4, 10, 10, 10, 10);
    double parallEnd = MPI_Wtime();
    double value = 9103.5;
    if (rank == 0) {
        ASSERT_LE(std::abs(parall - value), 1e-3);
        std::cout << "value:           " << value << std::endl;
        std::cout << "result:          " << integral << std::endl;
        std::cout << "result parall :  " << parall << std::endl;
        std::cout << std::fixed << std::setprecision(20) <<
            "integral :          " << integralEnd - integralStart <<
            std::endl;
        std::cout << std::fixed << std::setprecision(20) <<
            "parall integral :   " << parallEnd - parallStart <<
            std::endl;
    }
}

TEST(Integral, Test2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double integralStart = MPI_Wtime();
    double integral = integralFunction(function1, -1, 1.9, -1.3, 2, 2, 6.5, 10, 10, 100);
    double integralEnd = MPI_Wtime();
    double parallStart = MPI_Wtime();
    double parall = getCalculatedIntegral(function1, -1, 1.9, -1.3, 2, 2, 6.5, 10, 10, 100);
    double parallEnd = MPI_Wtime();
    double value = 28.826634375;
    if (rank == 0) {
        ASSERT_LE(std::abs(parall - value), 1e-3);
        std::cout << "value:           " << value << std::endl;
        std::cout << "result:          " << integral << std::endl;
        std::cout << "result parall :  " << parall << std::endl;
        std::cout << std::fixed << std::setprecision(20) <<
            "integral :          " << integralEnd - integralStart <<
            std::endl;
        std::cout << std::fixed << std::setprecision(20) <<
            "parall integral :   " << parallEnd - parallStart <<
            std::endl;
    }
}

TEST(Integral, Test3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double integralStart = MPI_Wtime();
    double integral = integralFunction(function3, 0, 1.8, -1.2, 1.1, -1, 2.9, 10, 10, 1000);
    double integralEnd = MPI_Wtime();
    double parallStart = MPI_Wtime();
    double parall = getCalculatedIntegral(function3, 0, 1.8, -1.2, 1.1, -1, 2.9, 10, 10, 1000);
    double parallEnd = MPI_Wtime();
    double value = 67.49028;
    if (rank == 0) {
        ASSERT_LE(std::abs(parall - value), 1e-3);
        std::cout << "value:           " << value << std::endl;
        std::cout << "result:          " << integral << std::endl;
        std::cout << "result parall :  " << parall << std::endl;
        std::cout << std::fixed << std::setprecision(20) <<
            "integral :          " << integralEnd - integralStart <<
            std::endl;
        std::cout << std::fixed << std::setprecision(20) <<
            "parall integral :   " << parallEnd - parallStart <<
            std::endl;
    }
}

TEST(Integral, Test4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double integralStart = MPI_Wtime();
    double integral = integralFunction(function3, 0.5, 1.7, -1, 2, -4, 1.9, 10, 10, 1000);
    double integralEnd = MPI_Wtime();
    double parallStart = MPI_Wtime();
    double parall = getCalculatedIntegral(function3, 0.5, 1.7, -1, 2, -4, 1.9, 10, 10, 1000);
    double parallEnd = MPI_Wtime();
    double value = -144.963;
    if (rank == 0) {
        ASSERT_LE(std::abs(parall - value), 1e-3);
        std::cout << "value:           " << value << std::endl;
        std::cout << "result:          " << integral << std::endl;
        std::cout << "result parall :  " << parall << std::endl;
        std::cout << std::fixed << std::setprecision(20) <<
            "integral :          " << integralEnd - integralStart <<
            std::endl;
        std::cout << std::fixed << std::setprecision(20) <<
            "parall integral :   " << parallEnd - parallStart <<
            std::endl;
    }
}

TEST(Integral, Test5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double integralStart = MPI_Wtime();
    double integral = integralFunction(function1, 0.1, 1, -1, 2, 4.2, 6, 10, 10, 100000);
    double integralEnd = MPI_Wtime();
    double parallStart = MPI_Wtime();
    double parall = getCalculatedIntegral(function1, 0.1, 1, -1, 2, 4.2, 6, 10, 10, 100000);
    double parallEnd = MPI_Wtime();
    double value = 6.81615;
    if (rank == 0) {
        ASSERT_LE(std::abs(parall - value), 1e-3);
        std::cout << "value:           " << value << std::endl;
        std::cout << "result:          " << integral << std::endl;
        std::cout << "result parall :  " << parall << std::endl;
        std::cout << std::fixed << std::setprecision(20) <<
            "integral :          " << integralEnd - integralStart <<
            std::endl;
        std::cout << std::fixed << std::setprecision(20) <<
            "parall integral :   " << parallEnd - parallStart <<
            std::endl;
    }
}

TEST(Integral, Test6) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double integralStart = MPI_Wtime();
    double integral = integralFunction(function1, 0.1, 1, -1.1, 2, -1, 5, 10, 10, 1000000);
    double integralEnd = MPI_Wtime();
    double parallStart = MPI_Wtime();
    double parall = getCalculatedIntegral(function1, 0.1, 1, -1.1, 2, -1, 5, 10, 10, 1000000);
    double parallEnd = MPI_Wtime();
    double value = 8.2863;
    if (rank == 0) {
        ASSERT_LE(std::abs(parall - value), 1e-3);
        std::cout << "value:           " << value << std::endl;
        std::cout << "result:          " << integral << std::endl;
        std::cout << "result parall :  " << parall << std::endl;
        std::cout << std::fixed << std::setprecision(20) <<
            "integral :          " << integralEnd - integralStart <<
            std::endl;
        std::cout << std::fixed << std::setprecision(20) <<
            "parall integral :   " << parallEnd - parallStart <<
            std::endl;
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
