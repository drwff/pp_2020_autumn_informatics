// Copyright 2020 Novozhilova Ekaterina
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./vector_sum.h"

TEST(Parallel_Sum_MPI, Test_30) {
    int Comm_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &Comm_rank);
    std::vector<int> vec;
    int sum, check_sum;
    int vec_size = 30;
    if (Comm_rank == 0) {
        vec = GenVector(vec_size);
        check_sum = GetSeqSum(vec);
    }
    sum = GetParSum(vec, vec_size);
    if (Comm_rank == 0) {
        ASSERT_EQ(check_sum, sum);
    }
}

TEST(Parallel_Sum_MPI, Test_500) {
    int Comm_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &Comm_rank);
    std::vector<int> vec;
    int sum, check_sum;
    int vec_size = 500;
    if (Comm_rank == 0) {
        vec = GenVector(vec_size);
        check_sum = GetSeqSum(vec);
    }
    sum = GetParSum(vec, vec_size);
    if (Comm_rank == 0) {
        ASSERT_EQ(check_sum, sum);
    }
}

TEST(Parallel_Sum_MPI, Test_1234) {
    int Comm_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &Comm_rank);
    std::vector<int> vec;
    int sum, check_sum;
    int vec_size = 1234;
    if (Comm_rank == 0) {
        vec = GenVector(vec_size);
        check_sum = GetSeqSum(vec);
    }
    sum = GetParSum(vec, vec_size);
    if (Comm_rank == 0) {
        ASSERT_EQ(check_sum, sum);
    }
}

TEST(Parallel_Sum_MPI, Test_85) {
    int Comm_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &Comm_rank);
    std::vector<int> vec;
    int sum, check_sum;
    int vec_size = 85;
    if (Comm_rank == 0) {
        vec = GenVector(vec_size);
        check_sum = GetSeqSum(vec);
    }
    sum = GetParSum(vec, vec_size);
    if (Comm_rank == 0) {
        ASSERT_EQ(check_sum, sum);
    }
}

TEST(Parallel_Sum_MPI, Test_777) {
    int Comm_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &Comm_rank);
    std::vector<int> vec;
    int sum, check_sum;
    int vec_size = 888;
    if (Comm_rank == 0) {
        vec = GenVector(vec_size);
        check_sum = GetSeqSum(vec);
    }
    sum = GetParSum(vec, vec_size);
    if (Comm_rank == 0) {
        ASSERT_EQ(check_sum, sum);
    }
}

TEST(Parallel_Sum_MPI, Test_1_2_3_4_5) {
    int Comm_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &Comm_rank);
    std::vector<int> vec(5);
    int sum, check_sum;
    vec = {1, 2, 3, 4, 5};
    if (Comm_rank == 0) {
        check_sum = 15;
    }
    sum = GetParSum(vec, 5);
    if (Comm_rank == 0) {
        ASSERT_EQ(check_sum, sum);
    }
}
int main(int argc, char**argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());
    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
