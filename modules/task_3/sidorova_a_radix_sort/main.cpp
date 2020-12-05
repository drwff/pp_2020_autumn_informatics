// Copyright 2020 Sidorova Alexandra

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <random>
#include <ctime>
#include <algorithm>
#include <vector>
#include <iostream>
#include "./radix_sort.h"

#define EPSILON 0.0001

TEST(RadixDoubleSortWithBatcher, DISABLED_TimeTest) {
    const size_t size = 100;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double starttime, endtime;
    std::vector<double> srcSeq;
    if (rank == 0) {
        srcSeq = getRandomVector(size);

        starttime = MPI_Wtime();
        radixSortSequential(srcSeq);
        endtime = MPI_Wtime();

        std::cout << "Sequential impl: " << endtime - starttime << std::endl;
    }

    std::vector<double> src;
    if (rank == 0)
        src = getRandomVector(size);

    starttime = MPI_Wtime();
    radixSortParallel(src);
    endtime = MPI_Wtime();
    if (rank == 0)
        std::cout << "Parallel impl: " << endtime - starttime << std::endl;
}

TEST(RadixDoubleSortWithBatcher, IncorrectSizeTest) {
    std::vector<double> src;

    ASSERT_ANY_THROW(radixSortSequential(src));
}

TEST(RadixDoubleSortWithBatcher, SequentialTest) {
    const size_t size = 10;
    std::vector<double> src = getRandomVector(size);
    std::vector<double> dst(size);
    std::vector<double> ref(src);

    dst = radixSortSequential(src);
    std::sort(ref.begin(), ref.end());

    for (size_t i = 0; i < size; i++)
        ASSERT_NEAR(dst[i], ref[i], EPSILON);
}

TEST(RadixDoubleSortWithBatcher, ParallelMediumSizeTest) {
    const size_t size = 20;
    std::vector<double> src;
    std::vector<double> ref;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        src = getRandomVector(size);
        ref = src;
    }

    std::vector<double> dst = radixSortParallel(src);

    if (rank == 0) {
        std::sort(ref.begin(), ref.end());

        for (size_t i = 0; i < size; i++)
            ASSERT_NEAR(dst[i], ref[i], EPSILON);
    }
}

TEST(RadixDoubleSortWithBatcher, ParallelSmallSizeTest) {
    const size_t size = 5;
    std::vector<double> src;
    std::vector<double> ref;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        src = getRandomVector(size);
        ref = src;
    }

    std::vector<double> dst = radixSortParallel(src);

    if (rank == 0) {
        std::sort(ref.begin(), ref.end());

        for (size_t i = 0; i < size; i++)
            ASSERT_NEAR(dst[i], ref[i], EPSILON);
    }
}

TEST(RadixDoubleSortWithBatcher, ParallelBigSizeTest) {
    const size_t size = 111;
    std::vector<double> src;
    std::vector<double> ref;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        src = getRandomVector(size);
        ref = src;
    }

    std::vector<double> dst = radixSortParallel(src);

    if (rank == 0) {
        std::sort(ref.begin(), ref.end());

        for (size_t i = 0; i < size; i++)
            ASSERT_NEAR(dst[i], ref[i], EPSILON);
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
