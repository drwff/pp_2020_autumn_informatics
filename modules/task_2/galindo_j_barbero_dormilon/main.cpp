// Copyright 2020 Galindo Buitrago Julio Cesar

#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./barbero_dormilon.h"

TEST(BarberoDormilon, Test1) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        ASSERT_ANY_THROW(ejecucion());
    }
}

TEST(BarberoDormilon, Test2) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size > 2) {
        ASSERT_NO_THROW(ejecucion());
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
