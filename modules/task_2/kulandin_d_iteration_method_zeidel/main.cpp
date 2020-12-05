// Copyright 2020 Kulandin Denis
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cmath>
#include <numeric>
#include <vector>
#include "./zeidel.h"

TEST(Parallel_MPI, Test_1) {
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    size_t n = 8;
    double eps = 1e-4;
    std::vector<double> a(n * n, 1);
    std::vector<double> b(n, 1);
    if (!procRank) {
        a = {
            61, 2,  3,  4,  6,  8,  9,  2,
            2, 84,  6,  4,  3,  2,  8,  7,
            3,  6, 68,  2,  4,  3,  9,  1,
            4,  4,  2, 59,  6,  4,  3,  8,
            6,  3,  4,  6, 93,  8,  3,  1,
            8,  2,  3,  4,  8, 98,  3,  1,
            9,  8,  9,  3,  3,  3, 85,  7,
            2,  7,  1,  8,  1,  1,  7, 98
        };

        b = {95, 116, 96, 90, 124, 127, 127, 125};
    }

    double timeParallel = MPI_Wtime();
    auto ans = zeidelParallel(a, b, n, eps);
    timeParallel = MPI_Wtime() - timeParallel;

    if (!procRank) {
        double timeSequential = MPI_Wtime();
        auto seq = zeidelSequential(a, b, n, eps);
        timeSequential = MPI_Wtime() - timeSequential;
        ASSERT_EQ(ans.first, seq.first);
        if (!ans.first) return;
        double err = 0;
        for (size_t i = 0; i < n; ++i) {
            double su = 0;
            for (size_t j = 0; j < n; ++j) {
                su += a[i * n + j] * ans.second[j];
            }
            err += std::pow(fabs(su - b[i]), 2);
        }
        ASSERT_LE(sqrt(err), eps);
        printf("Summary error:              %.10f\n", err);
        printf("Parallel time:              %.10f\n", timeParallel);
        printf("Sequential time:            %.10f\n", timeSequential);
    }
}

TEST(Parallel_MPI, Test_2) {
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    size_t n = 3;
    double eps = 1e-4;
    std::vector<double> a(n * n, 1);
    std::vector<double> b(n, 1);
    if (!procRank) {
        a = {
            10, 1, -1,
            1, 10, -1,
            -1, 1, 10
        };
        b = {11, 10, 10};
    }

    double timeParallel = MPI_Wtime();
    auto ans = zeidelParallel(a, b, n, eps);
    timeParallel = MPI_Wtime() - timeParallel;

    if (!procRank) {
        double timeSequential = MPI_Wtime();
        auto seq = zeidelSequential(a, b, n, eps);
        timeSequential = MPI_Wtime() - timeSequential;
        ASSERT_EQ(ans.first, seq.first);
        if (!ans.first) return;
        double err = 0;
        for (size_t i = 0; i < n; ++i) {
            double su = 0;
            for (size_t j = 0; j < n; ++j) {
                su += a[i * n + j] * ans.second[j];
            }
            err += std::pow(fabs(su - b[i]), 2);
        }
        ASSERT_LE(sqrt(err), eps);
        printf("Summary error:              %.10f\n", err);
        printf("Parallel time:              %.10f\n", timeParallel);
        printf("Sequential time:            %.10f\n", timeSequential);
    }
}


TEST(Parallel_MPI, Test_3) {
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    size_t n = 3;
    double eps = 1e-4;
    std::vector<double> a(n * n, 1);
    std::vector<double> b(n, 1);
    if (!procRank) {
        a = {
            115, -20, -75,
            15, -50, -5,
            6, 2, 20
        };
        b = {20, -40, 28};
    }

    double timeParallel = MPI_Wtime();
    auto ans = zeidelParallel(a, b, n, eps);
    timeParallel = MPI_Wtime() - timeParallel;

    if (!procRank) {
        double timeSequential = MPI_Wtime();
        auto seq = zeidelSequential(a, b, n, eps);
        timeSequential = MPI_Wtime() - timeSequential;
        ASSERT_EQ(ans.first, seq.first);
        if (!ans.first) return;
        double err = 0;
        for (size_t i = 0; i < n; ++i) {
            double su = 0;
            for (size_t j = 0; j < n; ++j) {
                su += a[i * n + j] * ans.second[j];
            }
            err += std::pow(fabs(su - b[i]), 2);
        }
        ASSERT_LE(sqrt(err), eps);
        printf("Summary error:              %.10f\n", err);
        printf("Parallel time:              %.10f\n", timeParallel);
        printf("Sequential time:            %.10f\n", timeSequential);
    }
}

TEST(Parallel_MPI, Test_4) {
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    size_t n = 100;
    double eps = 1e-4;
    std::vector<double> a(n * n, 1);
    std::vector<double> b(n, 1);
    if (!procRank) {
        a = randomVectorA(n);
        b = randomVectorB(n);
    }

    double timeParallel = MPI_Wtime();
    auto ans = zeidelParallel(a, b, n, eps);
    timeParallel = MPI_Wtime() - timeParallel;

    if (!procRank) {
        double timeSequential = MPI_Wtime();
        auto seq = zeidelSequential(a, b, n, eps);
        timeSequential = MPI_Wtime() - timeSequential;
        ASSERT_EQ(ans.first, seq.first);
        if (!ans.first) return;
        double err = 0;
        for (size_t i = 0; i < n; ++i) {
            double su = 0;
            for (size_t j = 0; j < n; ++j) {
                su += a[i * n + j] * ans.second[j];
            }
            err += std::pow(fabs(su - b[i]), 2);
        }
        ASSERT_LE(sqrt(err), eps);
        printf("Summary error:              %.10f\n", err);
        printf("Parallel time:              %.10f\n", timeParallel);
        printf("Sequential time:            %.10f\n", timeSequential);
    }
}

TEST(Parallel_MPI, Test_5) {
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    size_t n = 500;
    double eps = 1e-4;
    std::vector<double> a(n * n, 1);
    std::vector<double> b(n, 1);
    if (!procRank) {
        a = randomVectorA(n);
        b = randomVectorB(n);
    }

    double timeParallel = MPI_Wtime();
    auto ans = zeidelParallel(a, b, n, eps);
    timeParallel = MPI_Wtime() - timeParallel;

    if (!procRank) {
        double timeSequential = MPI_Wtime();
        auto seq = zeidelSequential(a, b, n, eps);
        timeSequential = MPI_Wtime() - timeSequential;
        ASSERT_EQ(ans.first, seq.first);
        if (!ans.first) return;
        double err = 0;
        for (size_t i = 0; i < n; ++i) {
            double su = 0;
            for (size_t j = 0; j < n; ++j) {
                su += a[i * n + j] * ans.second[j];
            }
            err += std::pow(fabs(su - b[i]), 2);
        }
        ASSERT_LE(sqrt(err), eps);
        printf("Summary error:              %.10f\n", err);
        printf("Parallel time:              %.10f\n", timeParallel);
        printf("Sequential time:            %.10f\n", timeSequential);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
