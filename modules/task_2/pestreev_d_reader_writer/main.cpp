// Copyright 2020 Pestreev Daniil
#include <mpi.h>
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

#include "../../../modules/task_2/pestreev_d_reader_writer/reader_writer.h"

TEST(Writers_Readers_Problem, writerCountIsInvalidOne) {
    int size, writer_count, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> data(0);
    writer_count = -1;
    if (rank == 0)
        std::cerr << "writer_count cannot be < 0\n";
    ASSERT_ANY_THROW(reader_writer(writer_count, data));
}

TEST(Writers_Readers_Problem, writerCountIsInvalidTwo) {
    int size, writer_count, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> data(0);
    writer_count = size;
    if (rank == 0)
        std::cerr << "writer_count cannot be > then (size - 2)\n";
    ASSERT_ANY_THROW(reader_writer(writer_count, data));
}

TEST(Writers_Readers_Problem, writerCountEqual2) {
    int size, writer_count, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    writer_count = 2;
    std::vector<int> data(0);
    if (writer_count < size - 2 && writer_count > 0) {
        ASSERT_GE(reader_writer(writer_count, data), 0);
    } else {
        ASSERT_ANY_THROW(reader_writer(writer_count, data));
    }
}

TEST(Writers_Readers_Problem, dataSizemod) {
    int size, writer_count, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    writer_count = 10;
    std::vector<int> data(0);
    if (writer_count < size - 2 && writer_count > 0) {
      int sizerw = reader_writer(writer_count, data);
      if (rank == 0) {
          int sizeD = writer_count - 1;
          ASSERT_EQ(sizerw, sizeD);
      }

    } else {
        ASSERT_ANY_THROW(reader_writer(writer_count, data));
    }
}

TEST(Writers_Readers_Problem, writerCountEqualNum) {
    int size, writer_count, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    writer_count = size / 2;
    std::vector<int> data(0);
    if (writer_count < size - 2 && writer_count > 0) {
        ASSERT_GE(reader_writer(writer_count, data), 0);
    } else {
        ASSERT_ANY_THROW(reader_writer(writer_count, data));
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
