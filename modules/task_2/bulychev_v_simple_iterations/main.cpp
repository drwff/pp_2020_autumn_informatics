// Copyright 2020 Bulychev Vladislav
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./simple_iterations.h"

TEST(Simple_Iterations, No_equal_size) {
    std::vector<double> mat{ 2, 1, 1, 1, 2, 1, 1, 1 };
    std::vector<double> vec{ 1, 1, 1 };
    ASSERT_ANY_THROW(Simple_iterations_Method(mat, vec, 0.0001));
}

TEST(Simple_Iterations, No_equal_size_MPI) {
    std::vector<double> mat{ 2, 1, 1, 1, 2, 1, 1, 1, 1 };
    std::vector<double> vec{ 1, 1 };
    ASSERT_ANY_THROW(MPI_Simple_iterations_Method(mat, vec, 0.0001));
}

TEST(Simple_Iterations, Zero_element_matrix) {
    std::vector<double> mat{ 2, 1, 1, 1, 2, 1, 1, 1, 0 };
    std::vector<double> vec{ 1, 1, 1 };
    ASSERT_ANY_THROW(Simple_iterations_Method(mat, vec, 0.0001));
}

TEST(Simple_Iterations, Zero_element_matrix_MPI) {
    std::vector<double> mat{ 2, 1, 1, 1, 2, 1, 1, 1, 0 };
    std::vector<double> vec{ 1, 1, 1 };
    ASSERT_ANY_THROW(MPI_Simple_iterations_Method(mat, vec, 0.0001));
}

TEST(Simple_Iterations, Simple_iterations_1) {
    std::vector<double> mat{ 4, 1, 1, 1, 6, -1, 1, 2, 5 };
    std::vector<double> vec{ 9, 10, 20 };
    std::vector<double> x1 = Simple_iterations_Method(mat, vec, 0.0001);
    std::vector<double> x2 = { 1.0001, 2, 3.0001 };
    int s = x1.size();
    for (int i = 0; i < s; i++) {
        ASSERT_NEAR(x1[i], x2[i], 0.0001);
    }
}

TEST(Simple_Iterations, Simple_iterations_2) {
    std::vector<double> mat{ 10, 1, 1, 2, 10, 1, 2, 2, 10 };
    std::vector<double> vec{ 12, 13, 14 };
    std::vector<double> x1 = Simple_iterations_Method(mat, vec, 0.0001);
    std::vector<double> x2 = { 1, 1, 1 };
    int s = x1.size();
    for (int i = 0; i < s; i++) {
        ASSERT_NEAR(x1[i], x2[i], 0.0001);
    }
}

TEST(Simple_Iterations, MPI_Simple_iterations_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> mat = { 10, 1, 1, 2, 10, 1, 2, 2, 10 };
    std::vector<double> vec = { 12, 13, 14 };
    std::vector<double> x1 = MPI_Simple_iterations_Method(mat, vec, 1e-5);
    if (rank == 0) {
        std::vector<double> x2 = Simple_iterations_Method(mat, vec, 1e-5);
        ASSERT_EQ(x1, x2);
    }
}

TEST(Simple_Iterations, MPI_Simple_iterations_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> mat = { 4, 1, 1, 1, 6, -1, 1, 2, 5 };
    std::vector<double> vec = { 9, 10, 20 };
    std::vector<double> x1 = MPI_Simple_iterations_Method(mat, vec, 1e-5);
    if (rank == 0) {
        std::vector<double> x2 = Simple_iterations_Method(mat, vec, 1e-5);
        ASSERT_EQ(x1, x2);
    }
}

TEST(Simple_Iterations, MPI_Simple_iterations_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> mat = { 32, 2, 1, 3, 1,
        1, 8, 3, 1, 3,
        1, 2, 16, 3, 1,
        1, 2, 3, 56, 1,
        1, 2, 1, 3, 32 };;
    std::vector<double> vec = { 43, 14, -3, 169, -19 };;
    std::vector<double> x1 = MPI_Simple_iterations_Method(mat, vec, 1e-5);
    if (rank == 0) {
        std::vector<double> x2 = Simple_iterations_Method(mat, vec, 1e-5);
        ASSERT_EQ(x1, x2);
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
