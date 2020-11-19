// Copyright 2020 Bakaeva Maria
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./unmached_signs.h"

TEST(UnmatchedSigns, differenceAtTheEnd) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int parallelCount = getParallelUnmachedSignsCount("abcdu", "abcdt");


    if (rank == 0) {
        int linearCount = getSequentialUnmachedSignsCount("abcdu", "abcdt");
        ASSERT_EQ(linearCount, parallelCount);
    }
}

TEST(UnmatchedSigns, DifferenceInTheMiddle) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int parallelCount = getParallelUnmachedSignsCount("abofpcd", "abfftcd");

    if (rank == 0) {
        int linearCount = getSequentialUnmachedSignsCount("abofpcd", "abfftcd");
        ASSERT_EQ(linearCount, parallelCount);
    }
}

TEST(UnmatchedSigns, equalsStrings) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int parallelCount = getParallelUnmachedSignsCount("abcd", "abcd");

    if (rank == 0) {
        int linearCount = getSequentialUnmachedSignsCount("abcd", "abcd");
        ASSERT_EQ(linearCount, parallelCount);
    }
}

TEST(UnmatchedSigns, differentLength) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ASSERT_ANY_THROW(getParallelUnmachedSignsCount("abcdopop", "abcd"));
    ASSERT_ANY_THROW(getSequentialUnmachedSignsCount("abcdopop", "abcd"));
}

TEST(UnmatchedSigns, RandomStrings) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int length = 10;
    char* str1 = generateString(length);
    char* str2 = generateString(length);
    int parallelCount = getParallelUnmachedSignsCount(str1, str2);

    if (rank == 0) {
        int linearCount = getSequentialUnmachedSignsCount(str1, str2);
        ASSERT_EQ(linearCount, parallelCount);
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
