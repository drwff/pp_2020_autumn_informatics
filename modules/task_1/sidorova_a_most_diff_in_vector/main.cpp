// Copyright 2020 Sidorova Alexandra

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./most_diff_in_vector.h"

TEST(VectorElementsWithMostDifference, CommonSearch) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const size_t sizeVector = 20;
    std::vector<int> vec(sizeVector);

    if (rank == 0)
        vec = getRandomVector(sizeVector);

    int maxDiff = getParallelSearch(vec);

    if (rank == 0) {
        int referenceMaxDiff = getSequentialSearch(vec);
        ASSERT_EQ(referenceMaxDiff, maxDiff);
    }
}

TEST(VectorElementsWithMostDifference, SearchInSmallVector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const size_t sizeVector = 2;
    std::vector<int> vec(sizeVector);

    if (rank == 0)
        vec = getRandomVector(sizeVector);

    int maxDiff = getParallelSearch(vec);

    if (rank == 0) {
        int referenceMaxDiff = getSequentialSearch(vec);
        ASSERT_EQ(referenceMaxDiff, maxDiff);
    }
}

TEST(VectorElementsWithMostDifference, SearchInNormalVector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const size_t sizeVector = 100;
    std::vector<int> vec(sizeVector);

    if (rank == 0)
        vec = getRandomVector(sizeVector);

    int maxDiff = getParallelSearch(vec);

    if (rank == 0) {
        int referenceMaxDiff = getSequentialSearch(vec);
        ASSERT_EQ(referenceMaxDiff, maxDiff);
    }
}

TEST(VectorElementsWithMostDifference, SearchInBigVector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const size_t sizeVector = 10000;
    std::vector<int> vec(sizeVector);

    if (rank == 0)
        vec = getRandomVector(sizeVector);

    int maxDiff = getParallelSearch(vec);

    if (rank == 0) {
        int referenceMaxDiff = getSequentialSearch(vec);
        ASSERT_EQ(referenceMaxDiff, maxDiff);
    }
}

TEST(VectorElementsWithMostDifference, SearchInVectorWithIncorrectSize) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const size_t sizeVector = 1;
    std::vector<int> vec(sizeVector);

    if (rank == 0)
        vec = getRandomVector(sizeVector);

    ASSERT_ANY_THROW(getParallelSearch(vec));
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
