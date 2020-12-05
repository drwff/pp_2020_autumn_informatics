// Copyright 2020 Alibekov Murad
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "iostream"
#include "./seidel_method.h"

TEST(Seidel_Method, my_SLAE_3_variables_sequential) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time, end_time;

    int size = 3;
    std::vector<double> A {
        10., 1., 2.,
        0., 10., 3.,
        0., 0., 5.
    };
    std::vector<double> b { 3., 7., 5. };
    std::vector<double> x(size);

    if (proc_rank == 0) {
        start_time = MPI_Wtime();
        x = solving_SLAE_sequential(A, b);
        end_time = MPI_Wtime();
    }

    if (proc_rank == 0) {
        std::vector<double> Ax(size);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                Ax[i] += A[i * size + j] * x[j];

        double error = d(Ax, b);
        printf("\tError = %.15f\n", error);
        printf("\tTime  = %f\n", end_time - start_time);

        ASSERT_LT(error, epsilon);
    }
}

TEST(Seidel_Method, my_SLAE_3_variables_parallel) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time, end_time;

    int size = 3;
    std::vector<double> A {
        10., 1., 2.,
        0., 10., 3.,
        0., 0., 5.
    };
    std::vector<double> b { 3., 7., 5. };
    std::vector<double> x(size);

    if (proc_rank == 0) start_time = MPI_Wtime();
    x = solving_SLAE_parallel(A, b);
    if (proc_rank == 0) end_time = MPI_Wtime();

    if (proc_rank == 0) {
        std::vector<double> Ax(size);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                Ax[i] += A[i * size + j] * x[j];

        double error = d(Ax, b);
        printf("\tError = %.15f\n", error);
        printf("\tTime  = %f\n", end_time - start_time);

        ASSERT_LT(error, epsilon);
    }
}

TEST(Seidel_Method, random_SLAE_5_variables_sequential) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time, end_time;

    int size = 5;
    std::vector<double> A(size * size);
    std::vector<double> b(size);
    std::vector<double> x(size);

    if (proc_rank == 0) {
        A = generate_A(size);
        b = generate_b(size);
    }

    if (proc_rank == 0) {
        start_time = MPI_Wtime();
        x = solving_SLAE_sequential(A, b);
        end_time = MPI_Wtime();
    }

    if (proc_rank == 0) {
        std::vector<double> Ax(size);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                Ax[i] += A[i * size + j] * x[j];

        double error = d(Ax, b);
        printf("\tError = %.15f\n", error);
        printf("\tTime  = %f\n", end_time - start_time);

        ASSERT_LT(error, epsilon);
    }
}

TEST(Seidel_Method, random_SLAE_5_variables_parallel) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time, end_time;

    int size = 5;
    std::vector<double> A(size * size);
    std::vector<double> b(size);
    std::vector<double> x(size);

    if (proc_rank == 0) {
        A = generate_A(size);
        b = generate_b(size);
    }

    if (proc_rank == 0) start_time = MPI_Wtime();
    x = solving_SLAE_parallel(A, b);
    if (proc_rank == 0) end_time = MPI_Wtime();

    if (proc_rank == 0) {
        std::vector<double> Ax(size);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                Ax[i] += A[i * size + j] * x[j];

        double error = d(Ax, b);
        printf("\tError = %.15f\n", error);
        printf("\tTime  = %f\n", end_time - start_time);

        ASSERT_LT(error, epsilon);
    }
}

TEST(Seidel_Method, random_SLAE_10_variables_sequential) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time, end_time;

    int size = 10;
    std::vector<double> A(size * size);
    std::vector<double> b(size);
    std::vector<double> x(size);

    if (proc_rank == 0) {
        A = generate_A(size);
        b = generate_b(size);
    }

    if (proc_rank == 0) {
        start_time = MPI_Wtime();
        x = solving_SLAE_sequential(A, b);
        end_time = MPI_Wtime();
    }

    if (proc_rank == 0) {
        std::vector<double> Ax(size);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                Ax[i] += A[i * size + j] * x[j];

        double error = d(Ax, b);
        printf("\tError = %.15f\n", error);
        printf("\tTime  = %f\n", end_time - start_time);

        ASSERT_LT(error, epsilon);
    }
}

TEST(Seidel_Method, random_SLAE_10_variables_parallel) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time, end_time;

    int size = 10;
    std::vector<double> A(size * size);
    std::vector<double> b(size);
    std::vector<double> x(size);

    if (proc_rank == 0) {
        A = generate_A(size);
        b = generate_b(size);
    }

    if (proc_rank == 0) start_time = MPI_Wtime();
    x = solving_SLAE_parallel(A, b);
    if (proc_rank == 0) end_time = MPI_Wtime();

    if (proc_rank == 0) {
        std::vector<double> Ax(size);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                Ax[i] += A[i * size + j] * x[j];

        double error = d(Ax, b);
        printf("\tError = %.15f\n", error);
        printf("\tTime  = %f\n", end_time - start_time);

        ASSERT_LT(error, epsilon);
    }
}

TEST(Seidel_Method, random_SLAE_50_variables_sequential) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time, end_time;

    int size = 50;
    std::vector<double> A(size * size);
    std::vector<double> b(size);
    std::vector<double> x(size);

    if (proc_rank == 0) {
        A = generate_A(size);
        b = generate_b(size);
    }

    if (proc_rank == 0) {
        start_time = MPI_Wtime();
        x = solving_SLAE_sequential(A, b);
        end_time = MPI_Wtime();
    }

    if (proc_rank == 0) {
        std::vector<double> Ax(size);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                Ax[i] += A[i * size + j] * x[j];

        double error = d(Ax, b);
        printf("\tError = %.15f\n", error);
        printf("\tTime  = %f\n", end_time - start_time);

        ASSERT_LT(error, epsilon);
    }
}

TEST(Seidel_Method, random_SLAE_50_variables_parallel) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time, end_time;

    int size = 50;
    std::vector<double> A(size * size);
    std::vector<double> b(size);
    std::vector<double> x(size);

    if (proc_rank == 0) {
        A = generate_A(size);
        b = generate_b(size);
    }

    if (proc_rank == 0) start_time = MPI_Wtime();
    x = solving_SLAE_parallel(A, b);
    if (proc_rank == 0) end_time = MPI_Wtime();

    if (proc_rank == 0) {
        std::vector<double> Ax(size);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                Ax[i] += A[i * size + j] * x[j];

        double error = d(Ax, b);
        printf("\tError = %.15f\n", error);
        printf("\tTime  = %f\n", end_time - start_time);

        ASSERT_LT(error, epsilon);
    }
}

TEST(Seidel_Method, random_SLAE_100_variables_sequential) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time, end_time;

    int size = 100;
    std::vector<double> A(size * size);
    std::vector<double> b(size);
    std::vector<double> x(size);

    if (proc_rank == 0) {
        A = generate_A(size);
        b = generate_b(size);
    }

    if (proc_rank == 0) {
        start_time = MPI_Wtime();
        x = solving_SLAE_sequential(A, b);
        end_time = MPI_Wtime();
    }

    if (proc_rank == 0) {
        std::vector<double> Ax(size);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                Ax[i] += A[i * size + j] * x[j];

        double error = d(Ax, b);
        printf("\tError = %.15f\n", error);
        printf("\tTime  = %f\n", end_time - start_time);

        ASSERT_LT(error, epsilon);
    }
}

TEST(Seidel_Method, random_SLAE_100_variables_parallel) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time, end_time;

    int size = 100;
    std::vector<double> A(size * size);
    std::vector<double> b(size);
    std::vector<double> x(size);

    if (proc_rank == 0) {
        A = generate_A(size);
        b = generate_b(size);
    }

    if (proc_rank == 0) start_time = MPI_Wtime();
    x = solving_SLAE_parallel(A, b);
    if (proc_rank == 0) end_time = MPI_Wtime();

    if (proc_rank == 0) {
        std::vector<double> Ax(size);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                Ax[i] += A[i * size + j] * x[j];

        double error = d(Ax, b);
        printf("\tError = %.15f\n", error);
        printf("\tTime  = %f\n", end_time - start_time);

        ASSERT_LT(error, epsilon);
    }
}

TEST(Seidel_Method, random_SLAE_500_variables_sequential) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time, end_time;

    int size = 500;
    std::vector<double> A(size * size);
    std::vector<double> b(size);
    std::vector<double> x(size);

    if (proc_rank == 0) {
        A = generate_A(size);
        b = generate_b(size);
    }

    if (proc_rank == 0) {
        start_time = MPI_Wtime();
        x = solving_SLAE_sequential(A, b);
        end_time = MPI_Wtime();
    }

    if (proc_rank == 0) {
        std::vector<double> Ax(size);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                Ax[i] += A[i * size + j] * x[j];

        double error = d(Ax, b);
        printf("\tError = %.15f\n", error);
        printf("\tTime  = %f\n", end_time - start_time);

        ASSERT_LT(error, epsilon);
    }
}

TEST(Seidel_Method, random_SLAE_500_variables_parallel) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time, end_time;

    int size = 500;
    std::vector<double> A(size * size);
    std::vector<double> b(size);
    std::vector<double> x(size);

    if (proc_rank == 0) {
        A = generate_A(size);
        b = generate_b(size);
    }

    if (proc_rank == 0) start_time = MPI_Wtime();
    x = solving_SLAE_parallel(A, b);
    if (proc_rank == 0) end_time = MPI_Wtime();

    if (proc_rank == 0) {
        std::vector<double> Ax(size);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                Ax[i] += A[i * size + j] * x[j];

        double error = d(Ax, b);
        printf("\tError = %.15f\n", error);
        printf("\tTime  = %f\n", end_time - start_time);

        ASSERT_LT(error, epsilon);
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
