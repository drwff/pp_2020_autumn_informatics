// Copyright 2020 Prokofeva Elizaveta
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "./lin_topology.h"

TEST(Lin_Topology_MPI, Test_Graph) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size == 1) {
        ASSERT_EQ(true, true);
        return;
    }

    MPI_Comm communicator = create_comm(MPI_COMM_WORLD);

    int nodes, edges;
    MPI_Graphdims_get(communicator, &nodes, &edges);
    ASSERT_EQ(nodes, size);
    ASSERT_EQ(edges, 2 * (size - 1));
}

TEST(Lin_Topology_MPI, Test_Graph_Neighbors) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size == 1) {
        ASSERT_EQ(true, true);
        return;
    }
    MPI_Comm communicator = create_comm(MPI_COMM_WORLD);
    MPI_Comm_rank(communicator, &rank);
    int count_neighbors;
    if ((rank == 0) || (rank == size - 1)) {
        MPI_Graph_neighbors_count(communicator, rank, &count_neighbors);
        ASSERT_EQ(count_neighbors, 1);
    } else {
        MPI_Graph_neighbors_count(communicator, rank, &count_neighbors);
        ASSERT_EQ(count_neighbors, 2);
    }
}

TEST(Lin_Topology_MPI, Test_Send_Recv_From_First_To_Last) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size == 1) {
        ASSERT_EQ(true, true);
        return;
    }
    MPI_Status status;
    MPI_Comm communicator = create_comm(MPI_COMM_WORLD);
    MPI_Comm_rank(communicator, &rank);

    int rank_s = 0;
    int rank_r = size - 1;

    int size_vec = 10;
    int* m = new int[size_vec];

    if (rank == rank_s) {
        m = getRandomVector(size_vec);
        MPI_Send(&m[0], size_vec, MPI_INT, rank_r, 0, MPI_COMM_WORLD);
    }
    if (rank == rank_r) {
        MPI_Recv(&m[0], size_vec, MPI_INT, rank_s, 0, MPI_COMM_WORLD, &status);
    }

    int* r = send_recv(&m[0], size_vec, MPI_INT, rank_r, 0, communicator, rank_s, rank);
    if (rank == rank_r) {
        std::vector<int> message(m, m + size_vec);
        std::vector<int> result_message(r, r + size_vec);
        ASSERT_EQ(message, result_message);
    }
}

TEST(Lin_Topology_MPI, Test_Send_Recv_From_First_To_Last_Big_Size) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size == 1) {
        ASSERT_EQ(true, true);
        return;
    }
    MPI_Status status;
    MPI_Comm communicator = create_comm(MPI_COMM_WORLD);
    MPI_Comm_rank(communicator, &rank);

    int rank_s = 0;
    int rank_r = size - 1;

    int size_vec = 100000;
    int* m = new int[size_vec];
    if (rank == rank_s) {
        m = getRandomVector(size_vec);
        MPI_Send(&m[0], size_vec, MPI_INT, rank_r, 0, MPI_COMM_WORLD);
    }
    if (rank == rank_r) {
        MPI_Recv(&m[0], size_vec, MPI_INT, rank_s, 0, MPI_COMM_WORLD, &status);
    }

    int* r = send_recv(&m[0], size_vec, MPI_INT, rank_r, 0, communicator, rank_s, rank);
    if (rank == rank_r) {
        std::vector<int> message(m, m + size_vec);
        std::vector<int> result_message(r, r + size_vec);
        ASSERT_EQ(message, result_message);
    }
}

TEST(Lin_Topology_MPI, Test_Send_Recv_From_Last_To_First) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size == 1) {
        ASSERT_EQ(true, true);
        return;
    }
    MPI_Status status;
    MPI_Comm communicator = create_comm(MPI_COMM_WORLD);
    MPI_Comm_rank(communicator, &rank);

    int rank_s = size - 1;
    int rank_r = 0;

    int size_vec = 10;
    int* m = new int[size_vec];
    if (rank == rank_s) {
        m = getRandomVector(size_vec);
        MPI_Send(&m[0], size_vec, MPI_INT, rank_r, 0, MPI_COMM_WORLD);
    }
    if (rank == rank_r) {
        MPI_Recv(&m[0], size_vec, MPI_INT, rank_s, 0, MPI_COMM_WORLD, &status);
    }

    int* r = send_recv(&m[0], size_vec, MPI_INT, rank_r, 0, communicator, rank_s, rank);
    if (rank == rank_r) {
        std::vector<int> message(m, m + size_vec);
        std::vector<int> result_message(r, r + size_vec);
        ASSERT_EQ(message, result_message);
    }
}

TEST(Lin_Topology_MPI, Test_Send_Recv_From_Last_To_First_Big_Size) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size == 1) {
        ASSERT_EQ(true, true);
        return;
    }
    MPI_Status status;
    MPI_Comm communicator = create_comm(MPI_COMM_WORLD);
    MPI_Comm_rank(communicator, &rank);

    int rank_s = size - 1;
    int rank_r = 0;

    int size_vec = 100000;
    int* m = new int[size_vec];

    if (rank == rank_s) {
        m = getRandomVector(size_vec);
        MPI_Send(&m[0], size_vec, MPI_INT, rank_r, 0, MPI_COMM_WORLD);
    }
    if (rank == rank_r) {
        MPI_Recv(&m[0], size_vec, MPI_INT, rank_s, 0, MPI_COMM_WORLD, &status);
    }

    int* r = send_recv(&m[0], size_vec, MPI_INT, rank_r, 0, communicator, rank_s, rank);
    if (rank == rank_r) {
        std::vector<int> message(m, m + size_vec);
        std::vector<int> result_message(r, r + size_vec);
        ASSERT_EQ(message, result_message);
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
