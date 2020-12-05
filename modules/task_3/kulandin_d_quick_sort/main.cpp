// Copyright 2020 Kulandin Denis
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "./quick_sort.h"

TEST(Parallel_Quick_sort, Test_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int N = 5;
    int* mas = new int[N];
    if (rank == 0) {
        mas = randomVector(N);
    }
    int* copy = copyVector(mas, N);

    double timeParallel = MPI_Wtime();
    parallelQuickSort(mas, N);
    timeParallel = MPI_Wtime() - timeParallel;

    if (rank == 0) {
        double timeSequential = MPI_Wtime();
        std::sort(copy, copy + N);
        timeSequential = MPI_Wtime() - timeSequential;
        ASSERT_EQ(std::vector<int>(copy, copy + N), std::vector<int>(mas, mas + N));
        printf("Size of array = %d\n", N);
        printf("Parallel time:              %.10f\n", timeParallel);
        printf("Sequential time:            %.10f\n", timeSequential);
    }
}

TEST(Parallel_Quick_sort, Test_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int N = 1337;
    int* mas = new int[N];
    if (rank == 0) {
        mas = randomVector(N);
    }
    int* copy = copyVector(mas, N);

    double timeParallel = MPI_Wtime();
    parallelQuickSort(mas, N);
    timeParallel = MPI_Wtime() - timeParallel;

    if (rank == 0) {
        double timeSequential = MPI_Wtime();
        std::sort(copy, copy + N);
        timeSequential = MPI_Wtime() - timeSequential;
        ASSERT_EQ(std::vector<int>(copy, copy + N), std::vector<int>(mas, mas + N));
        printf("Size of array = %d\n", N);
        printf("Parallel time:              %.10f\n", timeParallel);
        printf("Sequential time:            %.10f\n", timeSequential);
    }
}

TEST(Parallel_Quick_sort, Test_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int N = 123456;
    int* mas = new int[N];
    if (rank == 0) {
        mas = randomVector(N);
    }
    int* copy = copyVector(mas, N);

    double timeParallel = MPI_Wtime();
    parallelQuickSort(mas, N);
    timeParallel = MPI_Wtime() - timeParallel;

    if (rank == 0) {
        double timeSequential = MPI_Wtime();
        std::sort(copy, copy + N);
        timeSequential = MPI_Wtime() - timeSequential;
        ASSERT_EQ(std::vector<int>(copy, copy + N), std::vector<int>(mas, mas + N));
        printf("Size of array = %d\n", N);
        printf("Parallel time:              %.10f\n", timeParallel);
        printf("Sequential time:            %.10f\n", timeSequential);
    }
}

TEST(Parallel_Quick_sort, Test_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int N = 1864893;
    int* mas = new int[N];
    if (rank == 0) {
        mas = randomVector(N);
    }
    int* copy = copyVector(mas, N);

    double timeParallel = MPI_Wtime();
    parallelQuickSort(mas, N);
    timeParallel = MPI_Wtime() - timeParallel;

    if (rank == 0) {
        double timeSequential = MPI_Wtime();
        std::sort(copy, copy + N);
        timeSequential = MPI_Wtime() - timeSequential;
        ASSERT_EQ(std::vector<int>(copy, copy + N), std::vector<int>(mas, mas + N));
        printf("Size of array = %d\n", N);
        printf("Parallel time:              %.10f\n", timeParallel);
        printf("Sequential time:            %.10f\n", timeSequential);
    }
}

TEST(Parallel_Quick_sort, Test_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int N = 3424507;
    int* mas = new int[N];
    if (rank == 0) {
        mas = randomVector(N);
    }
    int* copy = copyVector(mas, N);

    double timeParallel = MPI_Wtime();
    parallelQuickSort(mas, N);
    timeParallel = MPI_Wtime() - timeParallel;

    if (rank == 0) {
        double timeSequential = MPI_Wtime();
        std::sort(copy, copy + N);
        timeSequential = MPI_Wtime() - timeSequential;
        ASSERT_EQ(std::vector<int>(copy, copy + N), std::vector<int>(mas, mas + N));
        printf("Size of array = %d\n", N);
        printf("Parallel time:              %.10f\n", timeParallel);
        printf("Sequential time:            %.10f\n", timeSequential);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
