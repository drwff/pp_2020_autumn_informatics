// Copyright 2020 Novozhilova Ekaterina
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./sleep_brad.h"

TEST(Parallel_Barber_MPI, Test_q_size_10) {
    // double t1 = MPI_Wtime();
    int Comm_rank, Comm_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &Comm_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Comm_size);
    const int queue_max_size = 10;
    MPI_Barrier(MPI_COMM_WORLD);
    int result = Barbershop(queue_max_size);
    MPI_Barrier(MPI_COMM_WORLD);
    // double t2 = MPI_Wtime();
    // std::cout << std::endl << t2 - t1 << std::endl;
    ASSERT_EQ(0, result);
}
TEST(Parallel_Barber_MPI, Test_q_size_3) {
    // double t1 = MPI_Wtime();
    int Comm_rank, Comm_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &Comm_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Comm_size);
    const int queue_max_size = 3;
    MPI_Barrier(MPI_COMM_WORLD);
    int result = Barbershop(queue_max_size);
    MPI_Barrier(MPI_COMM_WORLD);
    // double t2 = MPI_Wtime();
    // std::cout << std::endl << t2 - t1 << std::endl;
    ASSERT_EQ(0, result);
}
TEST(Parallel_Barber_MPI, Test_q_size_1) {
    // double t1 = MPI_Wtime();
    int Comm_rank, Comm_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &Comm_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Comm_size);
    const int queue_max_size = 1;
    MPI_Barrier(MPI_COMM_WORLD);
    int result = Barbershop(queue_max_size);
    MPI_Barrier(MPI_COMM_WORLD);
    // double t2 = MPI_Wtime();
    // std::cout << std::endl<< t2 - t1 << std::endl;
    ASSERT_EQ(0, result);
}
TEST(Parallel_Barber_MPI, Test_q_size_15) {
    // double t1 = MPI_Wtime();
    int Comm_rank, Comm_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &Comm_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Comm_size);
    const int queue_max_size = 15;
    MPI_Barrier(MPI_COMM_WORLD);
    int result = Barbershop(queue_max_size);
    MPI_Barrier(MPI_COMM_WORLD);
    // double t2 = MPI_Wtime();
    // std::cout << std::endl<< t2 - t1 << std::endl;
    ASSERT_EQ(0, result);
}
TEST(Parallel_Barber_MPI, Test_q_size_Comm_size) {
    // double t1 = MPI_Wtime();
    int Comm_rank, Comm_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &Comm_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Comm_size);
    const int queue_max_size = Comm_size;
    MPI_Barrier(MPI_COMM_WORLD);
    int result = Barbershop(queue_max_size);
    MPI_Barrier(MPI_COMM_WORLD);
    // double t2 = MPI_Wtime();
    // std::cout << std::endl<< t2 - t1 << std::endl;
    ASSERT_EQ(0, result);
}
int main(int argc, char**argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());
    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
