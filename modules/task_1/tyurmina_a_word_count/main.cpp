// Copyright 2020 Tyurmina Alexandra

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cmath>
#include <string>
#include "./word_count.h"
TEST(word_count, empty_string) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int res = string_not_correct("");
    if (0 == rank) {
        ASSERT_EQ(0, res);
    }
}
TEST(word_count, correct_string) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int res = string_not_correct("some one");
    if (0 == rank) {
        ASSERT_EQ(1, res);
    }
}

TEST(word_count, two_space) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int res = two_space("My father is a  doctor");
    if (0 == rank) {
        ASSERT_EQ(0, res);
    }
}

TEST(word_count, correct_count_word) {
    int rank = 0, result = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string s = "I also like to help my mother with my little sister very much";
    result = count_words(s, 1);
    if (0 == rank) {
        ASSERT_EQ(13, result);
    }
}

TEST(word_count, correct_count_word_paralell) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string s = "My mum enjoys reading and my dad enjoys playing chess with my brother Ken";
    int result = parallel_count_words(s, 0);
    if (0 == rank) {
        ASSERT_EQ(14, result);
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
