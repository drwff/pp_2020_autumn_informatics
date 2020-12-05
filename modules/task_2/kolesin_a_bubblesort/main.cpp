// Copyright 2020 Kolesin Andrey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <stdio.h>
#include <random>
#include <vector>
#include <algorithm>
#include "./bubblesort.h"

TEST(Count_Words, Test_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> arr = {1};
    parallelSort(&arr[0], arr.size());
    if (rank == 0) {
        EXPECT_EQ(arr, std::vector<int>({1}));
    }
}
TEST(Count_Words, Test_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> arr =  {-4, 10, 10, 10, 10};
    parallelSort(&arr[0], arr.size());
    if (rank == 0) {
        EXPECT_EQ(arr, std::vector<int>({-4, 10, 10, 10, 10}));
    }
}
TEST(Count_Words, Test_6) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> arr =  {-4, 10, 10, 10, 10, -4};
    parallelSort(&arr[0], arr.size());
    if (rank == 0) {
        EXPECT_EQ(arr, std::vector<int>({-4, -4, 10, 10, 10, 10}));
    }
}
TEST(Count_Words, Test_12) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> arr =  {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    parallelSort(&arr[0], arr.size());
    if (rank == 0) {
        EXPECT_EQ(arr, std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}));
    }
}
TEST(Count_Words, Test_13) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> arr = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    parallelSort(&arr[0], arr.size());
    if (rank == 0) {
        EXPECT_EQ(arr, std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}));
    }
}
TEST(Count_Words, Test_Random) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> arr = getRandomArray();
    std::vector<int> arr2 = arr;
    if (rank == 0) {
        parallelSort(&arr[0], arr.size());
        std::sort(arr2.begin(), arr2.end());
        EXPECT_EQ(arr, arr2);
    } else {
        int a;
        parallelSort(&a, 0);
    }
}
// TEST(Count_Words, Test_CompareTime) {
//     int rank;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     std::vector<int> arr = getRandomArray(10000);
//     std::vector<int> arr2 = arr;
//      if (rank == 0) {
//         double t1 = MPI_Wtime();
//         parallelSort(&arr[0],arr.size());
//         double t2 = MPI_Wtime();
//         SortMass(&arr2[0],arr2.size());
//         double t3 =  MPI_Wtime();
//         std::cout<<"seq: "<<t3-t2<<std::endl<<"paral: "<<t2-t1<<std::endl;
//         std::cout<<"s/p: "<<(t3-t2)/(t2-t1)<<std::endl;
//         EXPECT_EQ(arr, arr2);
//     }
//     else{
//         std::vector<int> a = {};
//         parallelSort(&arr[0],2);
//     }
// }
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
