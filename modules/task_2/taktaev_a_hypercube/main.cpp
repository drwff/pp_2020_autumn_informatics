// Copyright 2020 Taktaev Artem
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "../../../modules/task_2/taktaev_a_hypercube/hypercube.h"

TEST(Hypercube_MPI, Test_Wrong_Arg_In_getDimsCount) {
    ASSERT_EQ(getDimsCount(0), -1);
}

TEST(Hypercube_MPI, Test_procNum_Is_Not_Pow_Of_Two) {
    ASSERT_EQ(getDimsCount(15), -2);
}

TEST(Hypercube_MPI, Test_Correct_getDimsCount) {
    EXPECT_EQ(getDimsCount(16), 4);
}

TEST(Hypercube_MPI, Test_Wrong_Arg_In_createHypercubeTopology) {
     ASSERT_ANY_THROW(createHypercubeTopology(0));
}

TEST(Hypercube_MPI, Test_Data_Transfer) {
    int proc_num, proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);

    if (proc_num == 1) {
        ASSERT_EQ(1, 1);
        return;
    }
    int dim_count = getDimsCount(proc_num);
    if (dim_count == -2) {
        ASSERT_EQ(1, 1);
        return;
    }
    MPI_Comm hypercube = createHypercubeTopology(dim_count);
    int dest = 0 % proc_num;
    int source = 3 % proc_num;
    int send_data = 11, receive_data = 12;

    if (dest == source) {
        ASSERT_EQ(1, 1);
        return;
    }
    if (proc_rank == source)
        MPI_Send(&send_data, 1, MPI_INT, dest, 0, hypercube);
    if (proc_rank == dest) {
        MPI_Status status;
        MPI_Recv(&receive_data, 1, MPI_INT, source, 0, hypercube, &status);
        ASSERT_EQ(send_data, receive_data);
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
