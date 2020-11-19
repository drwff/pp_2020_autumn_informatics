// Copyright 2020 Tashirev Ivan
#include<mpi.h>
#include<gtest-mpi-listener.hpp>
#include<gtest/gtest.h>
#include<vector>
#include"./vector_min.h"

    TEST(Parralel_MPI, Min_10) {
        int ProcRank;
        MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
        const int size = 10;
        std::vector<int> vec(size);
        if (ProcRank == 0) {
            vec = getRandVector(size);
        }
        int parallel_min = getVectorMinValueParallel(vec, size);
        if (ProcRank == 0) {
            int min = getVectorMinValue(vec);
            ASSERT_EQ(min, parallel_min);
        }
    }
    TEST(Parralel_MPI, Min_100) {
        int ProcRank;
        MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
        const int size = 100;
        std::vector<int> vec(size);
        if (ProcRank == 0) {
            vec = getRandVector(size);
        }
        int parallel_min = getVectorMinValueParallel(vec, size);
        if (ProcRank == 0) {
            int min = getVectorMinValue(vec);
            ASSERT_EQ(min, parallel_min);
        }
    }
    TEST(Parralel_MPI, Min_500) {
        int ProcRank;
        MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
        const int size = 500;
        std::vector<int> vec(size);
        if (ProcRank == 0) {
            vec = getRandVector(size);
        }
        int parallel_min = getVectorMinValueParallel(vec, size);
        if (ProcRank == 0) {
            int min = getVectorMinValue(vec);
            ASSERT_EQ(min, parallel_min);
        }
    }
    TEST(Parralel_MPI, Min_101) {
        int ProcRank;
        MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
        const int size = 101;
        std::vector<int> vec(size);
        if (ProcRank == 0) {
            vec = getRandVector(size);
        }
        int parallel_min = getVectorMinValueParallel(vec, size);
        if (ProcRank == 0) {
            int min = getVectorMinValue(vec);
            ASSERT_EQ(min, parallel_min);
        }
    }
    TEST(Parralel_MPI, Min_60) {
        int ProcRank;
        MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
        const int size = 60;
        std::vector<int> vec(size);
        if (ProcRank == 0) {
            vec = getRandVector(size);
        }
        int parallel_min = getVectorMinValueParallel(vec, size);
        if (ProcRank == 0) {
            int min = getVectorMinValue(vec);
            ASSERT_EQ(min, parallel_min);
        }
    }

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
