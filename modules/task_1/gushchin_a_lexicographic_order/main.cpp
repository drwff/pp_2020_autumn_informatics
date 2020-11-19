// Copyright 2020 Gushchin Artem
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "./lexicographic_order.h"

TEST(Lexicographic_Order_MPI, Size_100_100) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::string string1, string2;
    const size_t length = 100;

    if (procRank == 0) {
        string1 = getRandomString(length);
        string2 = getRandomString(length);
    }

    int parallelResult = parallelIsLexicographicOrder(string1, string2, length, length);

    if (procRank == 0) {
        int sequentalResult = sequentialIsLexicographicOrder(string1, string2);
        EXPECT_EQ(parallelResult, sequentalResult);
    }
}

TEST(Lexicographic_Order_MPI, Size_71_97) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::string string1, string2;
    const size_t length1 = 71;
    const size_t length2 = 97;

    if (procRank == 0) {
        string1 = getRandomString(length1);
        string2 = getRandomString(length2);
    }

    int parallelResult = parallelIsLexicographicOrder(string1, string2, length1, length2);

    if (procRank == 0) {
        int sequentalResult = sequentialIsLexicographicOrder(string1, string2);
        EXPECT_EQ(parallelResult, sequentalResult);
    }
}

TEST(Lexicographic_Order_MPI, Size_61_43) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::string string1, string2;
    const size_t length1 = 61;
    const size_t length2 = 43;

    if (procRank == 0) {
        string1 = getRandomString(length1);
        string2 = getRandomString(length2);
    }

    int parallelResult = parallelIsLexicographicOrder(string1, string2, length1, length2);

    if (procRank == 0) {
        int sequentalResult = sequentialIsLexicographicOrder(string1, string2);
        EXPECT_EQ(parallelResult, sequentalResult);
    }
}

TEST(Lexicographic_Order_MPI, Equal_First_Larger) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::string string1, string2;
    const size_t length1 = 41;
    const size_t length2 = 16;

    if (procRank == 0) {
        string1 = "Equal beginnings, but first one is larger";
        string2 = "Equal beginnings";
    }

    int parallelResult = parallelIsLexicographicOrder(string1, string2, length1, length2);

    if (procRank == 0) {
        int sequentalResult = sequentialIsLexicographicOrder(string1, string2);
        EXPECT_EQ(parallelResult, sequentalResult);
    }
}

TEST(Lexicographic_Order_MPI, Equal_Second_Larger) {
    int procRank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::string string1, string2;
    const size_t length1 = 16;
    const size_t length2 = 42;

    if (procRank == 0) {
        string1 = "Equal beginnings";
        string2 = "Equal beginnings, but second one is larger";
    }

    int parallelResult = parallelIsLexicographicOrder(string1, string2, length1, length2);

    if (procRank == 0) {
        int sequentalResult = sequentialIsLexicographicOrder(string1, string2);
        EXPECT_EQ(parallelResult, sequentalResult);
    }
}

TEST(Lexicographic_Order_MPI, Throws_When_Size_0_0) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::string string1, string2;
    const size_t length1 = 0;
    const size_t length2 = 0;

    ASSERT_ANY_THROW(parallelIsLexicographicOrder(string1, string2, length1, length2));
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
