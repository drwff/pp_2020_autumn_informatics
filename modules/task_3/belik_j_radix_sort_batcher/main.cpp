// Copyright 2020 Belik Julia
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <numeric>
#include <algorithm>
#include "./RadixSortB.h"

TEST(Radix_Sort_Merge_Batcher, Test_Vector_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    const int n = 5;
    if (rank == 0) {
        vec = Vector(n);
    }
    std::vector<int> v1 = MergeBatcher(vec, n);
    if (rank == 0) {
        std::vector<int> v2 = RadixSort(vec);
        ASSERT_EQ(v1, v2);
    }
}
TEST(Radix_Sort_Merge_Batcher, Test_Vector_25) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    int n = 25;
    if (rank == 0) {
        vec = Vector(n);
    }
    std::vector<int> v1 = MergeBatcher(vec, n);
    if (rank == 0) {
        std::vector<int> v2 = RadixSort(vec);
        ASSERT_EQ(v1, v2);
    }
}
TEST(Radix_Sort_Merge_Batcher, Test_Vector_50) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    int n = 50;
    if (rank == 0) {
        vec = Vector(n);
    }
    std::vector<int> v1 = MergeBatcher(vec, n);
    if (rank == 0) {
        std::vector<int> v2 = RadixSort(vec);
        ASSERT_EQ(v1, v2);
    }
}
TEST(Radix_Sort_Merge_Batcher, Test_Vector_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    int n = 100;
    if (rank == 0) {
        vec = Vector(n);
    }
    std::vector<int> v1 = MergeBatcher(vec, n);
    if (rank == 0) {
        std::vector<int> v2 = RadixSort(vec);
        ASSERT_EQ(v1, v2);
    }
}
TEST(Radix_Sort_Merge_Batcher, Test_Vector_150) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    int n = 150;
    if (rank == 0) {
        vec = Vector(n);
    }
    std::vector<int> v1 = MergeBatcher(vec, n);
    if (rank == 0) {
        std::vector<int> v2 = RadixSort(vec);
        ASSERT_EQ(v1, v2);
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
