// Copyright 2020 Gurylev Nikita
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <random>
#include <vector>
#include "./gauss_jordan.h"

TEST(Parallel_MPI, TEST_1) {
    int n = 3, ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int nrows = getChunk(n, ProcNum, ProcRank);
    int* rows = new int[sizeof(*rows) * nrows];
    double* a = new double[sizeof(*a) * nrows * (n + 1)];
    double* x = new double[sizeof(*x) * n];
    double* tmp = new double[sizeof(*tmp) * (n + 1)];
    double t0, t1;
    std::mt19937 gen;
    for (int i = 0; i < nrows; i++) {
        rows[i] = ProcRank + ProcNum * i;
        gen.seed(rows[i] * (n + 1));
        for (int j = 0; j < n; j++)
            a[i * (n + 1) + j] = gen() % 100 + 1;
        a[i * (n + 1) + n] = gen() % 100 + 1;
    }
    t0 = MPI_Wtime();
    double* res = getParallelGaussJordan(n, nrows, rows, a, x, tmp);
    t1 = MPI_Wtime();
    if (ProcRank == 0) {
        for (int i = 0; i < n; i++) {
            std::cout << res[i] << std::endl;
        }
        std::cout << "ParallelMethodTime: " << t1 - t0 << std::endl;
    }
}

TEST(Parallel_MPI, TEST_2) {
    int n = 6, ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int nrows = getChunk(n, ProcNum, ProcRank);
    int* rows = new int[sizeof(*rows) * nrows];
    double* a = new double[sizeof(*a) * nrows * (n + 1)];
    double* x = new double[sizeof(*x) * n];
    double* tmp = new double[sizeof(*tmp) * (n + 1)];
    double t0, t1;
    std::mt19937 gen;
    for (int i = 0; i < nrows; i++) {
        rows[i] = ProcRank + ProcNum * i;
        gen.seed(rows[i] * (n + 1));
        for (int j = 0; j < n; j++)
            a[i * (n + 1) + j] = gen() % 100 + 1;
        a[i * (n + 1) + n] = gen() % 100 + 1;
    }
    t0 = MPI_Wtime();
    double* res = getParallelGaussJordan(n, nrows, rows, a, x, tmp);
    t1 = MPI_Wtime();
    if (ProcRank == 0) {
        for (int i = 0; i < n; i++) {
            std::cout << res[i] << std::endl;
        }
        std::cout << "ParallelMethodTime: " << t1 - t0 << std::endl;
    }
}

TEST(Parallel_MPI, TEST_3) {
    int n = 9, ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int nrows = getChunk(n, ProcNum, ProcRank);
    int* rows = new int[sizeof(*rows) * nrows];
    double* a = new double[sizeof(*a) * nrows * (n + 1)];
    double* x = new double[sizeof(*x) * n];
    double* tmp = new double[sizeof(*tmp) * (n + 1)];
    double t0, t1;
    std::mt19937 gen;
    for (int i = 0; i < nrows; i++) {
        rows[i] = ProcRank + ProcNum * i;
        gen.seed(rows[i] * (n + 1));
        for (int j = 0; j < n; j++)
            a[i * (n + 1) + j] = gen() % 100 + 1;
        a[i * (n + 1) + n] = gen() % 100 + 1;
    }
    t0 = MPI_Wtime();
    double* res = getParallelGaussJordan(n, nrows, rows, a, x, tmp);
    t1 = MPI_Wtime();
    if (ProcRank == 0) {
        for (int i = 0; i < n; i++) {
            std::cout << res[i] << std::endl;
        }
        std::cout << "ParallelMethodTime: " << t1 - t0 << std::endl;
    }
}

TEST(Parallel_MPI, TEST_4) {
    int n = 12, ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int nrows = getChunk(n, ProcNum, ProcRank);
    int* rows = new int[sizeof(*rows) * nrows];
    double* a = new double[sizeof(*a) * nrows * (n + 1)];
    double* x = new double[sizeof(*x) * n];
    double* tmp = new double[sizeof(*tmp) * (n + 1)];
    double t0, t1;
    std::mt19937 gen;
    for (int i = 0; i < nrows; i++) {
        rows[i] = ProcRank + ProcNum * i;
        gen.seed(rows[i] * (n + 1));
        for (int j = 0; j < n; j++)
            a[i * (n + 1) + j] = gen() % 100 + 1;
        a[i * (n + 1) + n] = gen() % 100 + 1;
    }
    t0 = MPI_Wtime();
    double* res = getParallelGaussJordan(n, nrows, rows, a, x, tmp);
    t1 = MPI_Wtime();
    if (ProcRank == 0) {
        for (int i = 0; i < n; i++) {
            std::cout << res[i] << std::endl;
        }
        std::cout << "ParallelMethodTime: " << t1 - t0 << std::endl;
    }
}

TEST(Parallel_MPI, TEST_5) {
    int n = 15, ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int nrows = getChunk(n, ProcNum, ProcRank);
    int* rows = new int[sizeof(*rows) * nrows];
    double* a = new double[sizeof(*a) * nrows * (n + 1)];
    double* x = new double[sizeof(*x) * n];
    double* tmp = new double[sizeof(*tmp) * (n + 1)];
    double t0, t1;
    std::mt19937 gen;
    for (int i = 0; i < nrows; i++) {
        rows[i] = ProcRank + ProcNum * i;
        gen.seed(rows[i] * (n + 1));
        for (int j = 0; j < n; j++)
            a[i * (n + 1) + j] = gen() % 100 + 1;
        a[i * (n + 1) + n] = gen() % 100 + 1;
    }
    t0 = MPI_Wtime();
    double* res = getParallelGaussJordan(n, nrows, rows, a, x, tmp);
    t1 = MPI_Wtime();
    if (ProcRank == 0) {
        for (int i = 0; i < n; i++) {
            std::cout << res[i] << std::endl;
        }
        std::cout << "ParallelMethodTime: " << t1 - t0 << std::endl;
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
