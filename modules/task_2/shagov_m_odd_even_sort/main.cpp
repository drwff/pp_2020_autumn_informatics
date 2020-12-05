// Copyright 2020 Shagov Maksim
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include "../../../modules/task_2/shagov_m_odd_even_sort/odd_even_sort.h"

TEST(Parallel_Odd_Even_Sort_MPI, Test_Random_Vector_Size_10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int count_size_string = 10;
    std::vector<int> global_array_1(count_size_string);
    std::vector<int> global_array_2(count_size_string);

    if (rank == 0) {
        global_array_1 = createRandomVector(count_size_string);
        global_array_2 = global_array_1;
    }
    oddEvenParallelSort(global_array_1.data(), count_size_string);

    if (rank == 0) {
        double startT = MPI_Wtime();
        bubbleSort(global_array_2.data(), global_array_2.size());
        double endT = MPI_Wtime();
        std::cout << "bubbleSort time: " << endT - startT << std::endl;
        ASSERT_EQ(global_array_1, global_array_2);
    }
}

TEST(Parallel_Odd_Even_Sort_MPI, Test_Random_Vector_Size_30) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int count_size_string = 30;
    std::vector<int> global_array_1(count_size_string);
    std::vector<int> global_array_2(count_size_string);

    if (rank == 0) {
        global_array_1 = createRandomVector(count_size_string);
        global_array_2 = global_array_1;
    }

    oddEvenParallelSort(global_array_1.data(), count_size_string);

    if (rank == 0) {
        double startT = MPI_Wtime();
        bubbleSort(global_array_2.data(), global_array_2.size());
        double endT = MPI_Wtime();
        std::cout << "bubbleSort time: " << endT - startT << std::endl;
        ASSERT_EQ(global_array_1, global_array_2);
    }
}

TEST(Parallel_Odd_Even_Sort_MPI, Test_Random_Vector_Size_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int count_size_string = 1000;
    std::vector<int> global_array_1(count_size_string);
    std::vector<int> global_array_2(count_size_string);

    if (rank == 0) {
        global_array_1 = createRandomVector(count_size_string);
        global_array_2 = global_array_1;
    }

    oddEvenParallelSort(global_array_1.data(), count_size_string);

    if (rank == 0) {
        double startT = MPI_Wtime();
        bubbleSort(global_array_2.data(), global_array_2.size());
        double endT = MPI_Wtime();
        std::cout << "bubbleSort time: " << endT - startT << std::endl;
        ASSERT_EQ(global_array_1, global_array_2);
    }
}

TEST(Parallel_Odd_Even_Sort_MPI, Test_Random_Vector_Size_113) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int count_size_string = 113;
    std::vector<int> global_array_1(count_size_string);
    std::vector<int> global_array_2(count_size_string);

    if (rank == 0) {
        global_array_1 = createRandomVector(count_size_string);
        global_array_2 = global_array_1;
    }

    oddEvenParallelSort(global_array_1.data(), count_size_string);

    if (rank == 0) {
        double startT = MPI_Wtime();
        bubbleSort(global_array_2.data(), global_array_2.size());
        double endT = MPI_Wtime();
        std::cout << "bubbleSort time: " << endT - startT << std::endl;
        ASSERT_EQ(global_array_1, global_array_2);
    }
}

TEST(Parallel_Odd_Even_Sort_MPI, Test_Random_Vector_Size_1235) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int count_size_string = 1235;
    std::vector<int> global_array_1(count_size_string);
    std::vector<int> global_array_2(count_size_string);

    if (rank == 0) {
        global_array_1 = createRandomVector(count_size_string);
        global_array_2 = global_array_1;
    }

    oddEvenParallelSort(global_array_1.data(), count_size_string);

    if (rank == 0) {
        double startT = MPI_Wtime();
        bubbleSort(global_array_2.data(), global_array_2.size());
        double endT = MPI_Wtime();
        std::cout << "bubbleSort time: " << endT - startT << std::endl;
        ASSERT_EQ(global_array_1, global_array_2);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
