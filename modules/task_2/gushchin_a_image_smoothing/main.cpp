// Copyright 2020 Gushchin Artem
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./image_smoothing.h"

TEST(Image_Smoothing_MPI, Size_1_1) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::vector<uchar> initial, reference, parallelResult;

    const int rows = 1, cols = 1, radius = 2;

    if (procRank == 0) {
        initial = { 83 };
        reference = { 83 };
    }

    parallelResult = parallelMedianFilter(initial, rows, cols, radius);

    if (procRank == 0) {
        std::vector<uchar> sequentalResult = medianFilter(initial, rows, cols, radius);
        EXPECT_EQ(parallelResult, reference);
        EXPECT_EQ(sequentalResult, reference);
    }
}

TEST(Image_Smoothing_MPI, Size_1_10) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::vector<uchar> initial, reference, parallelResult;

    const int rows = 1, cols = 10, radius = 1;

    if (procRank == 0) {
        initial = { 83, 199, 142, 28, 188, 231, 204, 170, 19, 171 };
        reference = { 83, 142, 142, 142, 188, 204, 204, 170, 170, 171 };
    }


    parallelResult = parallelMedianFilter(initial, rows, cols, radius);

    if (procRank == 0) {
        std::vector<uchar> sequentalResult = medianFilter(initial, rows, cols, radius);;
        EXPECT_EQ(parallelResult, reference);
        EXPECT_EQ(sequentalResult, reference);
    }
}

TEST(Image_Smoothing_MPI, Size_3_3) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::vector<uchar> initial, reference, parallelResult;

    const int rows = 3, cols = 3, radius = 1;

    if (procRank == 0) {
        initial = { 186,  31,   2,
                    250,  46, 110,
                      7, 168, 229 };

        reference = { 186,  46,  31,
                      168, 110, 110,
                       46, 168, 168 };
    }

    parallelResult = parallelMedianFilter(initial, rows, cols, radius);

    if (procRank == 0) {
        std::vector<uchar> sequentalResult = medianFilter(initial, rows, cols, radius);
        EXPECT_EQ(parallelResult, reference);
        EXPECT_EQ(sequentalResult, reference);
    }
}

TEST(Image_Smoothing_MPI, Size_7_6) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::vector<uchar> initial, reference, parallelResult;

    const int rows = 7, cols = 6, radius = 2;

    if (procRank == 0) {
        initial = {  16,  31,  78, 153,  27, 140,
                     36, 156, 179, 101, 250, 175,
                    241,  84, 207, 100,  51, 214,
                    241, 249, 193, 175,  58,  92,
                     95,  65,   6, 162,  18, 173,
                    149, 142,  60,  12,  44, 135,
                    251,  69,  89, 200,  28, 109 };

        reference = {  36,  78,  78, 101, 140, 140,
                       78, 101, 100, 140, 140, 140,
                       95, 101, 100, 140, 153, 153,
                      149, 149, 101, 135, 135, 135,
                      149, 149,  95,  92, 109, 109,
                      149, 149,  89,  89, 109, 109,
                      142, 142,  89,  89, 109, 109 };
    }

    parallelResult = parallelMedianFilter(initial, rows, cols, radius);

    if (procRank == 0) {
        std::vector<uchar> sequentalResult = medianFilter(initial, rows, cols, radius);
        EXPECT_EQ(parallelResult, reference);
        EXPECT_EQ(sequentalResult, reference);
    }
}

TEST(Image_Smoothing_MPI, Size_61_17_Random) {
    int procRank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<uchar> initial, reference, parallelResult;

    const int rows = 61, cols = 17, radius = 4;

    if (procRank == 0) {
        initial = generateRandomMatrix(rows, cols);
    }

    parallelResult = parallelMedianFilter(initial, rows, cols, radius);

    if (procRank == 0) {
        std::vector<uchar> sequentalResult = medianFilter(initial, rows, cols, radius);

        EXPECT_EQ(parallelResult, sequentalResult);
    }
}

TEST(Image_Smoothing_MPI, Throws_When_Size_0_0) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::vector<uchar> initial;
    const int rows = 0, cols = 0, radius = 2;

    ASSERT_ANY_THROW(parallelMedianFilter(initial, rows, cols, radius));
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
