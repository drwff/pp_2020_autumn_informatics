// Copyright 2020 Chistov Vladimir

#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./broadcast.h"


TEST(Parallel_Count_Sentences_MPI, Bcast_INT) {
    int ProcNum, ProcRank;
    int size = 5;
    int root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    std::vector<int> mas(size);
    std::vector<int> mas1(size);
    if (ProcRank == root) {
        for (int i = 0; i < size; i++) {
            mas[i] = i;
            mas1[i] = i;
        }
    }
    if (ProcRank != root) {
        for (int i = 0; i < size; i++) {
        }
    }
    MPI_Bcast(mas.data(), size, MPI_INT, root, MPI_COMM_WORLD);
    MY_Bcast(mas1.data(), size, MPI_INT, root, MPI_COMM_WORLD);
    if (ProcRank != root) {
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(mas[i], mas1[i]);
        }
    }
}

TEST(Parallel_Count_Sentences_MPI, Bcast_FLOAT) {
    int ProcNum, ProcRank;
    int size = 5;
    int root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    std::vector<float> mas(size);
    std::vector<float> mas1(size);
    if (ProcRank == root) {
        for (int i = 0; i < size; i++) {
            mas[i] = i + 0.1*i;
            mas1[i] = i + 0.1*i;
        }
    }
    if (ProcRank != root) {
        for (int i = 0; i < size; i++) {
        }
    }
    MPI_Bcast(mas.data(), size, MPI_FLOAT, root, MPI_COMM_WORLD);
    MY_Bcast(mas1.data(), size, MPI_FLOAT, root, MPI_COMM_WORLD);
    if (ProcRank != root) {
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(mas[i], mas1[i]);
        }
    }
}

TEST(Parallel_Count_Sentences_MPI, Bcast_DOUBLE) {
    int ProcNum, ProcRank;
    int size = 5;
    int root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    std::vector<double> mas(size);
    std::vector<double> mas1(size);
    if (ProcRank == root) {
        for (int i = 0; i < size; i++) {
            mas[i] = i + 0.1 * i;
            mas1[i] = i + 0.1 * i;
        }
    }
    if (ProcRank != root) {
        for (int i = 0; i < size; i++) {
        }
    }
    MPI_Bcast(mas.data(), size, MPI_DOUBLE, root, MPI_COMM_WORLD);
    MY_Bcast(mas1.data(), size, MPI_DOUBLE, root, MPI_COMM_WORLD);
    if (ProcRank != root) {
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(mas[i], mas1[i]);
        }
    }
}

TEST(Parallel_Count_Sentences_MPI, Bcast_FLOAT_BigSize) {
    int ProcNum, ProcRank;
    int size = 125;
    int root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    std::vector<float> mas(size);
    std::vector<float> mas1(size);
    if (ProcRank == root) {
        for (int i = 0; i < size; i++) {
            mas[i] = i + 0.1 * i;
            mas1[i] = i + 0.1 * i;
        }
    }
    if (ProcRank != root) {
        for (int i = 0; i < size; i++) {
        }
    }
    MPI_Bcast(mas.data(), size, MPI_FLOAT, root, MPI_COMM_WORLD);
    MY_Bcast(mas1.data(), size, MPI_FLOAT, root, MPI_COMM_WORLD);
    if (ProcRank != root) {
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(mas[i], mas1[i]);
        }
    }
}

TEST(Parallel_Count_Sentences_MPI, Bcast_DOUBLE_BigSize) {
    int ProcNum, ProcRank;
    int size = 125;
    int root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    std::vector<double> mas(size);
    std::vector<double> mas1(size);
    if (ProcRank == root) {
        for (int i = 0; i < size; i++) {
            mas[i] = i + 0.1*i;
            mas1[i] = i + 0.1*i;
        }
    }
    if (ProcRank != root) {
        for (int i = 0; i < size; i++) {
        }
    }
    MPI_Bcast(mas.data(), size, MPI_DOUBLE, root, MPI_COMM_WORLD);
    MY_Bcast(mas1.data(), size, MPI_DOUBLE, root, MPI_COMM_WORLD);
    if (ProcRank != root) {
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(mas[i], mas1[i]);
        }
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
