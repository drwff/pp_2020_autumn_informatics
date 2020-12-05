// Copyright 2020 Makarov Alexander
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./jacobi_method.h"

TEST(JacobiMethod, My_SLE_sequent) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        std::vector<double> A = {
            100., 19., -27.,
            -3., -78., 24.,
            18., 100., 274
        };
        std::vector<double> b = {
            357., -71., 44.,
        };

        double start_time = MPI_Wtime();
        std::vector<double> x = solveJacobiSequential(A, b);
        double end_time = MPI_Wtime();

        double error = calculateError(A, x, b);
        double time = end_time - start_time;
        std::cout << "\tError: " << std::fixed << error << " s" << std::endl;
        std::cout << "\tTime: " << std::fixed << time << std::endl;
        ASSERT_LE(error, epsilon);
    }
}

TEST(JacobiMethod, My_SLE_parallel) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> A;
    std::vector<double> b;
    if (rank == 0) {
        A = {
            100., 19., -27.,
            -3., -78., 24.,
            18., 100., 274.
        };
        b = {
            357., -71., 44.,
        };
    }
    double start_time, end_time;
    if (rank == 0) start_time = MPI_Wtime();
    std::vector<double> x = solveJacobiParallel(A, b);
    if (rank == 0) end_time = MPI_Wtime();

    if (rank == 0) {
        double error = calculateError(A, x, b);
        double time = end_time - start_time;
        std::cout << "\tError: " << std::fixed << error << " s" << std::endl;
        std::cout << "\tTime: " << std::fixed << time << std::endl;
        ASSERT_LE(error, epsilon);
    }
}

TEST(JacobiMethod, 10_SLE_sequent) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int size = 10;
        std::vector<double> A = generate_A(size);
        std::vector<double> b = generate_b(size);

        double start_time = MPI_Wtime();
        std::vector<double> x = solveJacobiSequential(A, b);
        double end_time = MPI_Wtime();

        double error = calculateError(A, x, b);
        double time = end_time - start_time;
        std::cout << "\tError: " << std::fixed << error << " s" << std::endl;
        std::cout << "\tTime: " << std::fixed << time << std::endl;
        ASSERT_LE(error, epsilon);
    }
}

TEST(JacobiMethod, 10_SLE_parallel) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> A;
    std::vector<double> b;
    if (rank == 0) {
        int size = 10;
        A = generate_A(size);
        b = generate_b(size);
    }
    double start_time, end_time;
    if (rank == 0) start_time = MPI_Wtime();
    std::vector<double> x = solveJacobiParallel(A, b);
    if (rank == 0) end_time = MPI_Wtime();

    if (rank == 0) {
        double error = calculateError(A, x, b);
        double time = end_time - start_time;
        std::cout << "\tError: " << std::fixed << error << " s" << std::endl;
        std::cout << "\tTime: " << std::fixed << time << std::endl;
        ASSERT_LE(error, epsilon);
    }
}

TEST(JacobiMethod, 100_SLE_sequent) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int size = 100;
        std::vector<double> A = generate_A(size);
        std::vector<double> b = generate_b(size);

        double start_time = MPI_Wtime();
        std::vector<double> x = solveJacobiSequential(A, b);
        double end_time = MPI_Wtime();

        double error = calculateError(A, x, b);
        double time = end_time - start_time;
        std::cout << "\tError: " << std::fixed << error << " s" << std::endl;
        std::cout << "\tTime: " << std::fixed << time << std::endl;
        ASSERT_LE(error, epsilon);
    }
}

TEST(JacobiMethod, 100_SLE_parallel) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> A;
    std::vector<double> b;
    if (rank == 0) {
        int size = 100;
        A = generate_A(size);
        b = generate_b(size);
    }
    double start_time, end_time;
    if (rank == 0) start_time = MPI_Wtime();
    std::vector<double> x = solveJacobiParallel(A, b);
    if (rank == 0) end_time = MPI_Wtime();

    if (rank == 0) {
        double error = calculateError(A, x, b);
        double time = end_time - start_time;
        std::cout << "\tError: " << std::fixed << error << " s" << std::endl;
        std::cout << "\tTime: " << std::fixed << time << std::endl;
        ASSERT_LE(error, epsilon);
    }
}

TEST(JacobiMethod, 1000_SLE_sequent) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int size = 1000;
        std::vector<double> A = generate_A(size);
        std::vector<double> b = generate_b(size);

        double start_time = MPI_Wtime();
        std::vector<double> x = solveJacobiSequential(A, b);
        double end_time = MPI_Wtime();

        double error = calculateError(A, x, b);
        double time = end_time - start_time;
        std::cout << "\tError: " << std::fixed << error << std::endl;
        std::cout << "\tTime: " << std::fixed << time << " s" << std::endl;
        ASSERT_LE(error, epsilon);
    }
}

TEST(JacobiMethod, 1000_SLE_parallel) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> A;
    std::vector<double> b;
    if (rank == 0) {
        int size = 1000;
        A = generate_A(size);
        b = generate_b(size);
    }
    double start_time, end_time;
    if (rank == 0) start_time = MPI_Wtime();
    std::vector<double> x = solveJacobiParallel(A, b);
    if (rank == 0) end_time = MPI_Wtime();

    if (rank == 0) {
        double error = calculateError(A, x, b);
        double time = end_time - start_time;
        std::cout << "\tError: " << std::fixed << error << " s" << std::endl;
        std::cout << "\tTime: " << std::fixed << time << std::endl;
        ASSERT_LE(error, epsilon);
    }
}

TEST(JacobiMethod, 5000_SLE_sequent) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int size = 5000;
        std::vector<double> A = generate_A(size);
        std::vector<double> b = generate_b(size);

        double start_time = MPI_Wtime();
        std::vector<double> x = solveJacobiSequential(A, b);
        double end_time = MPI_Wtime();

        double error = calculateError(A, x, b);
        double time = end_time - start_time;
        std::cout << "\tError: " << std::fixed << error << std::endl;
        std::cout << "\tTime: " << std::fixed << time << " s" << std::endl;
        ASSERT_LE(error, epsilon);
    }
}

TEST(JacobiMethod, 5000_SLE_parallel) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> A;
    std::vector<double> b;
    if (rank == 0) {
        int size = 5000;
        A = generate_A(size);
        b = generate_b(size);
    }
    double start_time, end_time;
    if (rank == 0) start_time = MPI_Wtime();
    std::vector<double> x = solveJacobiParallel(A, b);
    if (rank == 0) end_time = MPI_Wtime();

    if (rank == 0) {
        double error = calculateError(A, x, b);
        double time = end_time - start_time;
        std::cout << "\tError: " << std::fixed << error << " s" << std::endl;
        std::cout << "\tTime: " << std::fixed << time << std::endl;
        ASSERT_LE(error, epsilon);
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
