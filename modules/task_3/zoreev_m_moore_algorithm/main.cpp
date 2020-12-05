// Copyright 2020 Zoreev Mikhail

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>

#include "../../../modules/task_3/zoreev_m_moore_algorithm/moore_algorithm.h"

TEST(Moore_Algotithm_MPI, SpeedAndQualtiyTestTemplate) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t size = 8;
    int64_t *graph = new int64_t[size * size];
    int64_t *seqential_distance = nullptr;
    uint64_t *seqential_preducessor = nullptr;
    if (rank == 0) {
        randomCompleteGraph(size, graph);
        printGraph(size, graph);  // <- Remove on speed test
        double begin_time = MPI_Wtime();
        seqential_distance = new int64_t[size];
        seqential_preducessor = new uint64_t[size];
        mooreAlgorithm(size, graph, seqential_distance, seqential_preducessor, 0);
        double end_time = MPI_Wtime();
        std::cout << "Seqential: " << end_time - begin_time << std::endl;
        printPredecessor(size, seqential_preducessor);  // <- Remove on speed test
    }
    MPI_Bcast(graph, static_cast<int>(size * size), MPI_UINT64_T, 0, MPI_COMM_WORLD);
    double begin_time = MPI_Wtime();
    int64_t *parallel_distance = new int64_t[size];
    uint64_t *parallel_preducessor = new uint64_t[size];
    mooreAlgorithmParallel(size, graph, parallel_distance, parallel_preducessor, 0);
    double end_time = MPI_Wtime();
    if (rank == 0) {
        std::cout << "Parallel: " << end_time - begin_time << std::endl;
        printPredecessor(size, parallel_preducessor);  // <- Remove on speed test
        for (size_t i = 0; i < size; i++) {
            ASSERT_EQ(seqential_distance[i], parallel_distance[i]);
        }
        delete[] seqential_distance;
        delete[] seqential_preducessor;
    }

    delete[] graph;
    delete[] parallel_distance;
    delete[] parallel_preducessor;
}

TEST(Moore_Algotithm_MPI, TestCompleGraph8) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t size = 8;
    int64_t *graph = new int64_t[size * size];
    int64_t *seqential_distance = nullptr;
    uint64_t *seqential_preducessor = nullptr;
    if (rank == 0) {
        randomCompleteGraph(size, graph);
        double begin_time = MPI_Wtime();
        seqential_distance = new int64_t[size];
        seqential_preducessor = new uint64_t[size];
        mooreAlgorithm(size, graph, seqential_distance, seqential_preducessor, 0);
        double end_time = MPI_Wtime();
        std::cout << "Seqential: " << end_time - begin_time << std::endl;
    }
    MPI_Bcast(graph, static_cast<int>(size * size), MPI_UINT64_T, 0, MPI_COMM_WORLD);
    double begin_time = MPI_Wtime();
    int64_t *parallel_distance = new int64_t[size];
    uint64_t *parallel_preducessor = new uint64_t[size];
    mooreAlgorithmParallel(size, graph, parallel_distance, parallel_preducessor, 0);
    double end_time = MPI_Wtime();
    if (rank == 0) {
        std::cout << "Parallel: " << end_time - begin_time << std::endl;
        for (size_t i = 0; i < size; i++) {
            ASSERT_EQ(seqential_distance[i], parallel_distance[i]);
        }
        delete[] seqential_distance;
        delete[] seqential_preducessor;
    }

    delete[] graph;
    delete[] parallel_distance;
    delete[] parallel_preducessor;
}

TEST(Moore_Algotithm_MPI, TestCompleGraph16) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t size = 16;
    int64_t *graph = new int64_t[size * size];
    int64_t *seqential_distance = nullptr;
    uint64_t *seqential_preducessor = nullptr;
    if (rank == 0) {
        randomCompleteGraph(size, graph);
        double begin_time = MPI_Wtime();
        seqential_distance = new int64_t[size];
        seqential_preducessor = new uint64_t[size];
        mooreAlgorithm(size, graph, seqential_distance, seqential_preducessor, 0);
        double end_time = MPI_Wtime();
        std::cout << "Seqential: " << end_time - begin_time << std::endl;
    }
    MPI_Bcast(graph, static_cast<int>(size * size), MPI_UINT64_T, 0, MPI_COMM_WORLD);
    double begin_time = MPI_Wtime();
    int64_t *parallel_distance = new int64_t[size];
    uint64_t *parallel_preducessor = new uint64_t[size];
    mooreAlgorithmParallel(size, graph, parallel_distance, parallel_preducessor, 0);
    double end_time = MPI_Wtime();
    if (rank == 0) {
        std::cout << "Parallel: " << end_time - begin_time << std::endl;
        for (size_t i = 0; i < size; i++) {
            ASSERT_EQ(seqential_distance[i], parallel_distance[i]);
        }
        delete[] seqential_distance;
        delete[] seqential_preducessor;
    }

    delete[] graph;
    delete[] parallel_distance;
    delete[] parallel_preducessor;
}

TEST(Moore_Algotithm_MPI, TestCompleGraph32) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t size = 32;
    int64_t *graph = new int64_t[size * size];
    int64_t *seqential_distance = nullptr;
    uint64_t *seqential_preducessor = nullptr;
    if (rank == 0) {
        randomCompleteGraph(size, graph);
        double begin_time = MPI_Wtime();
        seqential_distance = new int64_t[size];
        seqential_preducessor = new uint64_t[size];
        mooreAlgorithm(size, graph, seqential_distance, seqential_preducessor, 0);
        double end_time = MPI_Wtime();
        std::cout << "Seqential: " << end_time - begin_time << std::endl;
    }
    MPI_Bcast(graph, static_cast<int>(size * size), MPI_UINT64_T, 0, MPI_COMM_WORLD);
    double begin_time = MPI_Wtime();
    int64_t *parallel_distance = new int64_t[size];
    uint64_t *parallel_preducessor = new uint64_t[size];
    mooreAlgorithmParallel(size, graph, parallel_distance, parallel_preducessor, 0);
    double end_time = MPI_Wtime();
    if (rank == 0) {
        std::cout << "Parallel: " << end_time - begin_time << std::endl;
        for (size_t i = 0; i < size; i++) {
            ASSERT_EQ(seqential_distance[i], parallel_distance[i]);
        }
        delete[] seqential_distance;
        delete[] seqential_preducessor;
    }

    delete[] graph;
    delete[] parallel_distance;
    delete[] parallel_preducessor;
}

TEST(Moore_Algotithm_MPI, TestGraphWithPath64) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t size = 64;
    int64_t *graph = new int64_t[size * size];
    int64_t *seqential_distance = nullptr;
    uint64_t *seqential_preducessor = nullptr;
    if (rank == 0) {
        randomGraphWithPath(size, graph);
        double begin_time = MPI_Wtime();
        seqential_distance = new int64_t[size];
        seqential_preducessor = new uint64_t[size];
        mooreAlgorithm(size, graph, seqential_distance, seqential_preducessor, 0);
        double end_time = MPI_Wtime();
        std::cout << "Seqential: " << end_time - begin_time << std::endl;
    }
    MPI_Bcast(graph, static_cast<int>(size * size), MPI_UINT64_T, 0, MPI_COMM_WORLD);
    double begin_time = MPI_Wtime();
    int64_t *parallel_distance = new int64_t[size];
    uint64_t *parallel_preducessor = new uint64_t[size];
    mooreAlgorithmParallel(size, graph, parallel_distance, parallel_preducessor, 0);
    double end_time = MPI_Wtime();
    if (rank == 0) {
        std::cout << "Parallel: " << end_time - begin_time << std::endl;
        for (size_t i = 0; i < size; i++) {
            ASSERT_EQ(seqential_distance[i], parallel_distance[i]);
        }
        delete[] seqential_distance;
        delete[] seqential_preducessor;
    }

    delete[] graph;
    delete[] parallel_distance;
    delete[] parallel_preducessor;
}

TEST(Moore_Algotithm_MPI, TestGraphWithPath101) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t size = 101;
    int64_t *graph = new int64_t[size * size];
    int64_t *seqential_distance = nullptr;
    uint64_t *seqential_preducessor = nullptr;
    if (rank == 0) {
        randomGraphWithPath(size, graph);
        double begin_time = MPI_Wtime();
        seqential_distance = new int64_t[size];
        seqential_preducessor = new uint64_t[size];
        mooreAlgorithm(size, graph, seqential_distance, seqential_preducessor, 0);
        double end_time = MPI_Wtime();
        std::cout << "Seqential: " << end_time - begin_time << std::endl;
    }
    MPI_Bcast(graph, static_cast<int>(size * size), MPI_UINT64_T, 0, MPI_COMM_WORLD);
    double begin_time = MPI_Wtime();
    int64_t *parallel_distance = new int64_t[size];
    uint64_t *parallel_preducessor = new uint64_t[size];
    mooreAlgorithmParallel(size, graph, parallel_distance, parallel_preducessor, 0);
    double end_time = MPI_Wtime();
    if (rank == 0) {
        std::cout << "Parallel: " << end_time - begin_time << std::endl;
        for (size_t i = 0; i < size; i++) {
            ASSERT_EQ(seqential_distance[i], parallel_distance[i]);
        }
        delete[] seqential_distance;
        delete[] seqential_preducessor;
    }

    delete[] graph;
    delete[] parallel_distance;
    delete[] parallel_preducessor;
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
