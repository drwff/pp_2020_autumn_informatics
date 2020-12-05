// Copyright 2020 Tarasov Nikita
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_3/tarasov_n_quick_sort/quick_sort.h"

TEST(Quick_sort_MPI, test1) {
    int mpirank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);
    if (mpirank == 0) {
        ASSERT_ANY_THROW(random_array(-5));
    }
}

TEST(Quick_sort_MPI, test2) {
    int size = 0;
    std::vector<int> a(size);
    int mpirank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);
    if (mpirank == 0) {
        ASSERT_ANY_THROW(quick_sort(&a, 0 , 0));
    }
}

TEST(Quick_sort_MPI, test3) {
    int size = 1;
    std::vector<int> a(size);
    int mpirank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);
    if (mpirank == 0) {
        a = random_array(size);
        std::vector<int> ans = a;
        quick_sort(&ans, 0, size - 1);
        ASSERT_EQ(a, ans);
    }
}

TEST(Quick_sort_MPI, test4) {
    int size = 10;
    std::vector <int> a(size), ans_seq(size);
    int mpirank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);
    if (mpirank == 0) {
        a[0] = 41;
        a[1] = 67;
        a[2] = 34;
        a[3] = 0;
        a[4] = 69;
        a[5] = 24;
        a[6] = 78;
        a[7] = 58;
        a[8] = 62;
        a[9] = 64;
        ans_seq[0] = 0;
        ans_seq[1] = 24;
        ans_seq[2] = 34;
        ans_seq[3] = 41;
        ans_seq[4] = 58;
        ans_seq[5] = 62;
        ans_seq[6] = 64;
        ans_seq[7] = 67;
        ans_seq[8] = 69;
        ans_seq[9] = 78;
    }
    std::vector<int> ans = a;
    quick_sort_mpi(&ans);
    if (mpirank == 0) {
        ASSERT_EQ(ans_seq, ans);
    }
}

TEST(Quick_sort_MPI, test5) {
    int size = 10;
    std::vector <int> a(size), ans_seq(size);
    int mpirank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);
    if (mpirank == 0) {
        a[0] = 41;
        a[1] = 67;
        a[2] = 34;
        a[3] = 0;
        a[4] = 69;
        a[5] = 24;
        a[6] = 78;
        a[7] = 58;
        a[8] = 62;
        a[9] = 64;
        ans_seq[0] = 0;
        ans_seq[1] = 24;
        ans_seq[2] = 34;
        ans_seq[3] = 41;
        ans_seq[4] = 58;
        ans_seq[5] = 62;
        ans_seq[6] = 64;
        ans_seq[7] = 67;
        ans_seq[8] = 69;
        ans_seq[9] = 78;
    }
    std::vector <int> ans = a;
    quick_sort(&ans, 0, size - 1);
    if (mpirank == 0) {
        ASSERT_EQ(ans_seq, ans);
    }
}

TEST(Quick_sort_MPI, test6) {
    int size = 100;
    std::vector <int> a(size);
    int mpirank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);
    if (mpirank == 0) {
        a = random_array(size);
    }
    std::vector <int> ans = a;
    quick_sort_mpi(&ans);
    std::vector <int> ans_seq = a;
    if (mpirank == 0) {
        quick_sort(&ans_seq, 0, size - 1);
        ASSERT_EQ(ans_seq, ans);
    }
}

TEST(Quick_sort_MPI, test7) {
    int size = 1000;
    std::vector <int> a(size);
    int mpirank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);
    if (mpirank == 0) {
        a = random_array(size);
    }
    std::vector <int> ans = a;
    quick_sort_mpi(&ans);
    std::vector <int> ans_seq = a;
    if (mpirank == 0) {
        quick_sort(&ans_seq, 0, size - 1);
        ASSERT_EQ(ans_seq, ans);
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
