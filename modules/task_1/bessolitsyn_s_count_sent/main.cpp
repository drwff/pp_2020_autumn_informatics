// Copyright 2020 Bessolitsyn Sergey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <stdio.h>
#include <random>
#include <vector>
#include "../../../modules/task_1/bessolitsyn_s_count_sent/count_sent.h"

TEST(Parallel_Operations_MPI, Test_Size_30) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char> global_vec;
    const int count_size_vector = 30;
    if (rank == 0) {
        global_vec = getRandomString(count_size_vector);
    }
    int global_res = getParallelOperations(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_res = getSequentialOperations(global_vec, count_size_vector);
        std::cout << global_res << " " << reference_res << std::endl;
        ASSERT_EQ(reference_res, global_res);
    }
}

TEST(Parallel_Operations_MPI, Test_Size_200) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char> global_vec;
    const int count_size_vector = 200;
    if (rank == 0) {
        global_vec = getRandomString(count_size_vector);
    }
    int global_res = getParallelOperations(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_res = getSequentialOperations(global_vec, count_size_vector);
        std::cout << global_res << " " << reference_res << std::endl;
        ASSERT_EQ(reference_res, global_res);
    }
}

TEST(Parallel_Operations_MPI, Test_Size_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char> global_vec;
    const int count_size_vector = 1000;
    if (rank == 0) {
        global_vec = getRandomString(count_size_vector);
    }
    int global_res = getParallelOperations(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_res = getSequentialOperations(global_vec, count_size_vector);
        std::cout << global_res << " " << reference_res << std::endl;
        ASSERT_EQ(reference_res, global_res);
    }
}

TEST(Parallel_Operations_MPI, Test_Size_1003) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char> global_vec;
    const int count_size_vector = 1003;
    if (rank == 0) {
        global_vec = getRandomString(count_size_vector);
    }
    int global_res = getParallelOperations(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_res = getSequentialOperations(global_vec, count_size_vector);
        std::cout << global_res << " " << reference_res << std::endl;
        ASSERT_EQ(reference_res, global_res);
    }
}

TEST(Parallel_Operations_MPI, Test_Size_3541) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char> global_vec;
    const int count_size_vector = 3541;
    if (rank == 0) {
        global_vec = getRandomString(count_size_vector);
    }
    int global_res = getParallelOperations(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_res = getSequentialOperations(global_vec, count_size_vector);
        std::cout << global_res << " " << reference_res << std::endl;
        ASSERT_EQ(reference_res, global_res);
    }
}

TEST(Parallel_Operations_MPI, Test_Size_10000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char> global_vec;
    const int count_size_vector = 10000;
    if (rank == 0) {
        global_vec = getRandomString(count_size_vector);
    }
    int global_res = getParallelOperations(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_res = getSequentialOperations(global_vec, count_size_vector);
        std::cout << global_res << " " << reference_res << std::endl;
        ASSERT_EQ(reference_res, global_res);
    }
}

TEST(Parallel_Operations_MPI, Test_Size_10000_Points) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int count_size_vector = 10000;
    std::vector<char> global_vec;
    if (rank == 0) {
        global_vec.resize(count_size_vector);
        for (int i = 0; i < count_size_vector; ++i) {
            global_vec[i] = '.';
        }
    }
    int global_res = getParallelOperations(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_res = getSequentialOperations(global_vec, count_size_vector);
        std::cout << global_res << " " << reference_res << std::endl;
        ASSERT_EQ(reference_res, global_res);
    }
}

TEST(Parallel_Operations_MPI, Test_Size_100000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char> global_vec;
    const int count_size_vector = 100000;
    if (rank == 0) {
        global_vec = getRandomString(count_size_vector);
    }
    int global_res = getParallelOperations(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_res = getSequentialOperations(global_vec, count_size_vector);
        std::cout << global_res << " " << reference_res << std::endl;
        ASSERT_EQ(reference_res, global_res);
    }
}

TEST(Parallel_Operations_MPI, Test_Size_1000000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<char> global_vec;
    const int count_size_vector = 1000000;
    if (rank == 0) {
        global_vec = getRandomString(count_size_vector);
    }
    int global_res = getParallelOperations(global_vec, count_size_vector);
    if (rank == 0) {
        int reference_res = getSequentialOperations(global_vec, count_size_vector);
        std::cout << global_res << " " << reference_res << std::endl;
        ASSERT_EQ(reference_res, global_res);
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
