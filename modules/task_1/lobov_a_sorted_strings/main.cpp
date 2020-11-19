// Copyright 2020 Lobov Alexander
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <string>
#include "./sorted_strings.h"

TEST(Parallel_Operations_MPI, Test_randomStringSize) {
    std::string s1 = getRandomString(10);
    EXPECT_EQ(10, (int)s1.size());
}

TEST(Parallel_Operations_MPI, Test_isSortedWhenEq) {
    std::string s1 = "abc";
    std::string s2 = "abc";
    EXPECT_TRUE(isSortedStringPair(s1, s2));
}

TEST(Parallel_Operations_MPI, Test_isSortedWhenSorted) {
    std::string s1 = "abc";
    std::string s2 = "abc1";
    EXPECT_TRUE(isSortedStringPair(s1, s2));
}

TEST(Parallel_Operations_MPI, Test_isSortedWhenNotSorted) {
    std::string s1 = "abcd";
    std::string s2 = "abc";
    EXPECT_FALSE(isSortedStringPair(s1, s2));
}

TEST(Parallel_Operations_MPI, Test_bigSize1) {
    std::string s1 = "abcKADJNiufjrfoicmdshidpadkpkcsoijdoihfiuhsfugidufjfiakdpapokdxosaxcsdcijfojsoifspdjfosjja";
    std::string s2 = "abcKADJNiufjrfoicmdshidpadkpkcsoijdoihfiuhsfugidufjfiakdpapokdxosaxcsdcijfojsoifspdjfosjjt";
    EXPECT_EQ(isSortedStringPair(s1, s2), isSortedStringPairParallel(s1, s2));
}


TEST(Parallel_Operations_MPI, Test_bigSize) {
    std::string s1 = "abcKADJNiufjrfoicmdshidpadkpkcsoijdoihfiuhsfugidufjfiakdpapokdxosaxcsdcijfojsoifspdjfosjjt";
    std::string s2 = "abcKADJNiufjrfoicmdshidpadkpkcsoijdoihfiuhsfugidufjfiakdpapokdxosaxcsdcijfojsoifspdjfosjjt";
    EXPECT_TRUE(isSortedStringPairParallel(s1, s2));
}

TEST(Parallel_Operations_MPI, Test_bigSize3) {
    std::string s1 = "";
    std::string s2 = "";
    EXPECT_TRUE(isSortedStringPairParallel(s1, s2));
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
