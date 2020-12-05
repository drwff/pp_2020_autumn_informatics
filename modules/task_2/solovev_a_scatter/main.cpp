// Copyright 2020 Solovev Alexandr
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <random>
#include <vector>
#include "../../../modules/task_2/solovev_a_scatter/scatter.h"

TEST(MPI_Scatter, int_to_int) {
    const int root = 0;
    int rank, size;
    int k = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> data(30);
    std::vector<int> data1(30);
    std::vector<int> data2(30);
    int data_size = static_cast<int>(data.size());
    std::vector<int> recv(data_size/ size);
    int recv_size = static_cast<int>(recv.size());
    std::vector<int> scatter(data_size / size);
    int scatter_size = static_cast<int>(scatter.size());

    for (int& i : data) {
        i = ++k;
    }
    My_Scatter(data.data(), data_size / size, MPI_INT, recv.data(),
               data_size / size, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Scatter(data.data(), data_size / size, MPI_INT, scatter.data(),
               data_size / size, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Gather(recv.data(), recv_size, MPI_INT,
        data1.data(), recv_size, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Gather(scatter.data(), scatter_size, MPI_INT,
        data2.data(), scatter_size, MPI_INT, root, MPI_COMM_WORLD);

    if (rank == root) {
        ASSERT_EQ(data1, data2);
    }
}


TEST(MPI_Scatter, float_to_float) {
    const int root = 0;
    int rank, size;
    float k = 3.3f;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<float> data(35);
    int data_size = static_cast<int>(data.size());
    std::vector<float> data1(35);
    std::vector<float> data2(35);
    std::vector<float> recv(data_size / size);
    int recv_size = static_cast<int>(recv.size());
    std::vector<float> scatter(data_size / size);
    int scatter_size = static_cast<int>(scatter.size());
    for (float& i : data) {
        i = ++k;
    }
    My_Scatter(data.data(), data_size / size, MPI_FLOAT, recv.data(),
               data_size / size, MPI_FLOAT, root, MPI_COMM_WORLD);
    MPI_Scatter(data.data(), data_size / size, MPI_FLOAT, scatter.data(),
               data_size / size, MPI_FLOAT, root, MPI_COMM_WORLD);
    MPI_Gather(recv.data(), recv_size, MPI_FLOAT,
        data1.data(), recv_size, MPI_FLOAT, root, MPI_COMM_WORLD);
    MPI_Gather(scatter.data(), scatter_size, MPI_FLOAT,
        data2.data(), scatter_size, MPI_FLOAT, root, MPI_COMM_WORLD);

    if (rank == root) {
        ASSERT_EQ(data1, data2);
    }
}

TEST(MPI_Scatter, double_to_double) {
    const int root = 0;
    int rank, size;
    double k = 17.3;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> data(35);
    int data_size = static_cast<int>(data.size());
    std::vector<double> data1(35);
    std::vector<double> data2(35);
    std::vector<double> recv(data_size / size);
    int recv_size = static_cast<int>(recv.size());
    std::vector<double> scatter(data_size / size);
    int scatter_size = static_cast<int>(scatter.size());
    for (double& i : data) {
        i = ++k;
    }
    My_Scatter(data.data(), data_size / size, MPI_DOUBLE, recv.data(),
               data_size / size, MPI_DOUBLE, root, MPI_COMM_WORLD);
    MPI_Scatter(data.data(), data_size / size, MPI_DOUBLE, scatter.data(),
               data_size / size, MPI_DOUBLE, root, MPI_COMM_WORLD);
    MPI_Gather(recv.data(), recv_size, MPI_DOUBLE,
        data1.data(), recv_size, MPI_DOUBLE, root, MPI_COMM_WORLD);
    MPI_Gather(scatter.data(), scatter_size, MPI_DOUBLE,
        data2.data(), scatter_size, MPI_DOUBLE, root, MPI_COMM_WORLD);

    if (rank == root) {
        ASSERT_EQ(data1, data2);
    }
}
TEST(MPI_Scatter, char_to_char) {
    const int root = 0;
    int rank, size;
    char k = '1';
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<char> data(40);
    int data_size = static_cast<int>(data.size());
    std::vector<char> data1(40);
    std::vector<char> data2(40);
    std::vector<char> recv(data_size / size);
    int recv_size = static_cast<int>(recv.size());
    std::vector<char> scatter(data_size / size);
    int scatter_size = static_cast<int>(scatter.size());
    for (char& i : data) {
        i = ++k;
    }
    My_Scatter(data.data(), data_size / size, MPI_CHAR, recv.data(),
               data_size / size, MPI_CHAR, root, MPI_COMM_WORLD);
    MPI_Scatter(data.data(), data_size / size, MPI_CHAR, scatter.data(),
               data_size / size, MPI_CHAR, root, MPI_COMM_WORLD);
    MPI_Gather(recv.data(), recv_size, MPI_CHAR,
        data1.data(), recv_size, MPI_CHAR, root, MPI_COMM_WORLD);
    MPI_Gather(scatter.data(), scatter_size, MPI_CHAR,
        data2.data(), scatter_size, MPI_CHAR, root, MPI_COMM_WORLD);

    if (rank == root) {
        ASSERT_EQ(data1, data2);
    }
}

TEST(MPI_Scatter, error_count_send) {
    const int root = 0;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> data(20);
    int data_size = static_cast<int>(data.size());
    std::vector<char> recv(data_size / size);

    ASSERT_EQ(My_Scatter(data.data(), -1, MPI_INT, recv.data(),
               data_size / size, MPI_INT, root, MPI_COMM_WORLD), MPI_ERR_COUNT);
}

TEST(MPI_Scatter, error_count_recv) {
    const int root = 0;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> data(20);
    int data_size = static_cast<int>(data.size());
    std::vector<char> recv(data_size / size);

    ASSERT_EQ(My_Scatter(data.data(), data_size, MPI_INT, recv.data(),
               -1, MPI_INT, root, MPI_COMM_WORLD), MPI_ERR_COUNT);
}

TEST(MPI_Scatter, error_both_count) {
    const int root = 0;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> data(20);
    int data_size = static_cast<int>(data.size());
    std::vector<char> recv(data_size / size);
    int recv_size = static_cast<int>(recv.size());

    ASSERT_EQ(My_Scatter(data.data(), data_size, MPI_INT, recv.data(),
               recv_size, MPI_INT, root, MPI_COMM_WORLD), MPI_ERR_COUNT);
}

TEST(MPI_Scatter, error_data) {
    const int root = 0;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> data(20);
    int data_size = static_cast<int>(data.size());
    std::vector<int> recv(20);
    int recv_size = static_cast<int>(recv.size());

    ASSERT_EQ(My_Scatter(nullptr, data_size, MPI_INT, recv.data(),
                recv_size, MPI_INT, root, MPI_COMM_WORLD), MPI_ERR_BUFFER);
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
