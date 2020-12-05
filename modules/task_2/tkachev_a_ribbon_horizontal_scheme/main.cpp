// Copyright 2020 Tkachev Alexey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <limits>
#include "../../../../modules/task_2/tkachev_a_ribbon_horizontal_scheme/ribbon_horizontal_scheme.h"
using std::vector;

TEST(Ribbon_horizontal_scheme, MatrixTest_10_50) {
    int process_rank;
    int matrix_count_rows = 10, matrix_count_columns = 50;
    std::vector<int> test_matrix(matrix_count_rows * matrix_count_columns);

    std::vector<int> the_test_vector(matrix_count_columns);

    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    if (process_rank == 0) {
       test_matrix = getRandomVector(matrix_count_rows, matrix_count_columns);
       the_test_vector = getRandomVector(matrix_count_columns, 1);
    }

    std::vector<int> parallel_multiply_on_vector;

    double _time1 = MPI_Wtime();
    parallel_multiply_on_vector = parallelMultiplyOnVector(
                test_matrix, the_test_vector, matrix_count_rows, matrix_count_columns);
    double _time2 = MPI_Wtime();

    if (process_rank == 0) {
        printf("PARRALEL TIME: %.5f\n", _time2-_time1);
        std::vector<int> multiply_on_vector;
        _time1 = MPI_Wtime();
        multiply_on_vector = multiplyOnVector(test_matrix, the_test_vector,
        matrix_count_rows, matrix_count_columns);
        _time2 = MPI_Wtime();
        printf("NOT PARRALEL TIME %.5f\n", _time2-_time1);
        ASSERT_EQ(multiply_on_vector, parallel_multiply_on_vector);
    }
}

TEST(Ribbon_horizontal_scheme, MatrixTest_53_41) {
    int process_rank;
    int matrix_count_rows = 53, matrix_count_columns = 41;
    std::vector<int> test_matrix(matrix_count_rows * matrix_count_columns);

    std::vector<int> the_test_vector(matrix_count_columns);

    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    if (process_rank == 0) {
       test_matrix = getRandomVector(matrix_count_rows, matrix_count_columns);
       the_test_vector = getRandomVector(matrix_count_columns, 1);
    }

    std::vector<int> parallel_multiply_on_vector;

    double _time1 = MPI_Wtime();
    parallel_multiply_on_vector = parallelMultiplyOnVector(
                test_matrix, the_test_vector, matrix_count_rows, matrix_count_columns);
    double _time2 = MPI_Wtime();

    if (process_rank == 0) {
        printf("PARRALEL TIME: %.5f\n", _time2-_time1);
        std::vector<int> multiply_on_vector;
        _time1 = MPI_Wtime();
        multiply_on_vector = multiplyOnVector(test_matrix, the_test_vector,
        matrix_count_rows, matrix_count_columns);
        _time2 = MPI_Wtime();
        printf("NOT PARRALEL TIME %.5f\n", _time2-_time1);
        ASSERT_EQ(multiply_on_vector, parallel_multiply_on_vector);
    }
}

TEST(Ribbon_horizontal_scheme, MatrixTest_1_1) {
    int process_rank;
    int matrix_count_rows = 1, matrix_count_columns = 1;
    std::vector<int> test_matrix(matrix_count_rows * matrix_count_columns);

    std::vector<int> the_test_vector(matrix_count_columns);

    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    if (process_rank == 0) {
       test_matrix = getRandomVector(matrix_count_rows, matrix_count_columns);
       the_test_vector = getRandomVector(matrix_count_columns, 1);
    }

    std::vector<int> parallel_multiply_on_vector;

    double _time1 = MPI_Wtime();
    parallel_multiply_on_vector = parallelMultiplyOnVector(
                test_matrix, the_test_vector, matrix_count_rows, matrix_count_columns);
    double _time2 = MPI_Wtime();

    if (process_rank == 0) {
        printf("PARRALEL TIME: %.5f\n", _time2-_time1);
        std::vector<int> multiply_on_vector;
        _time1 = MPI_Wtime();
        multiply_on_vector = multiplyOnVector(test_matrix, the_test_vector,
        matrix_count_rows, matrix_count_columns);
        _time2 = MPI_Wtime();
        printf("NOT PARRALEL TIME %.5f\n", _time2-_time1);
        ASSERT_EQ(multiply_on_vector, parallel_multiply_on_vector);
    }
}

TEST(Ribbon_horizontal_scheme, MatrixTest_2_2) {
    int process_rank;
    int matrix_count_rows = 2, matrix_count_columns = 2;
    std::vector<int> test_matrix(matrix_count_rows * matrix_count_columns);

    std::vector<int> the_test_vector(matrix_count_columns);

    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    if (process_rank == 0) {
       test_matrix = getRandomVector(matrix_count_rows, matrix_count_columns);
       the_test_vector = getRandomVector(matrix_count_columns, 1);
    }

    std::vector<int> parallel_multiply_on_vector;

    double _time1 = MPI_Wtime();
    parallel_multiply_on_vector = parallelMultiplyOnVector(
                test_matrix, the_test_vector, matrix_count_rows, matrix_count_columns);
    double _time2 = MPI_Wtime();

    if (process_rank == 0) {
        printf("PARRALEL TIME: %.5f\n", _time2-_time1);
        std::vector<int> multiply_on_vector;
        _time1 = MPI_Wtime();
        multiply_on_vector = multiplyOnVector(test_matrix, the_test_vector,
        matrix_count_rows, matrix_count_columns);
        _time2 = MPI_Wtime();
        printf("NOT PARRALEL TIME %.5f\n", _time2-_time1);
        ASSERT_EQ(multiply_on_vector, parallel_multiply_on_vector);
    }
}

TEST(Ribbon_horizontal_scheme, MatrixTest_10_20) {
    int process_rank;
    int matrix_count_rows = 10, matrix_count_columns = 20;
    std::vector<int> test_matrix(matrix_count_rows * matrix_count_columns);

    std::vector<int> the_test_vector(matrix_count_columns);

    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    if (process_rank == 0) {
       test_matrix = getRandomVector(matrix_count_rows, matrix_count_columns);
       the_test_vector = getRandomVector(matrix_count_columns, 1);
    }

    std::vector<int> parallel_multiply_on_vector;

    double _time1 = MPI_Wtime();
    parallel_multiply_on_vector = parallelMultiplyOnVector(
                test_matrix, the_test_vector, matrix_count_rows, matrix_count_columns);
    double _time2 = MPI_Wtime();

    if (process_rank == 0) {
        printf("PARRALEL TIME: %.5f\n", _time2-_time1);
        std::vector<int> multiply_on_vector;
        _time1 = MPI_Wtime();
        multiply_on_vector = multiplyOnVector(test_matrix, the_test_vector,
        matrix_count_rows, matrix_count_columns);
        _time2 = MPI_Wtime();
        printf("NOT PARRALEL TIME %.5f\n", _time2-_time1);
        ASSERT_EQ(multiply_on_vector, parallel_multiply_on_vector);
    }
}


TEST(Ribbon_horizontal_scheme, MatrixTest_20_10) {
    int process_rank;
    int matrix_count_rows = 20, matrix_count_columns = 10;
    std::vector<int> test_matrix(matrix_count_rows * matrix_count_columns);

    std::vector<int> the_test_vector(matrix_count_columns);

    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    if (process_rank == 0) {
       test_matrix = getRandomVector(matrix_count_rows, matrix_count_columns);
       the_test_vector = getRandomVector(matrix_count_columns, 1);
    }

    std::vector<int> parallel_multiply_on_vector;

    double _time1 = MPI_Wtime();
    parallel_multiply_on_vector = parallelMultiplyOnVector(
                test_matrix, the_test_vector, matrix_count_rows, matrix_count_columns);
    double _time2 = MPI_Wtime();

    if (process_rank == 0) {
        printf("PARRALEL TIME: %.5f\n", _time2-_time1);
        std::vector<int> multiply_on_vector;
        _time1 = MPI_Wtime();
        multiply_on_vector = multiplyOnVector(test_matrix, the_test_vector,
        matrix_count_rows, matrix_count_columns);
        _time2 = MPI_Wtime();
        printf("NOT PARRALEL TIME %.5f\n", _time2-_time1);
        ASSERT_EQ(multiply_on_vector, parallel_multiply_on_vector);
    }
}

TEST(Ribbon_horizontal_scheme, MatrixTest_51_49) {
    int process_rank;
    int matrix_count_rows = 51, matrix_count_columns = 49;
    std::vector<int> test_matrix(matrix_count_rows * matrix_count_columns);

    std::vector<int> the_test_vector(matrix_count_columns);

    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    if (process_rank == 0) {
       test_matrix = getRandomVector(matrix_count_rows, matrix_count_columns);
       the_test_vector = getRandomVector(matrix_count_columns, 1);
    }

    std::vector<int> parallel_multiply_on_vector;

    double _time1 = MPI_Wtime();
    parallel_multiply_on_vector = parallelMultiplyOnVector(
                test_matrix, the_test_vector, matrix_count_rows, matrix_count_columns);
    double _time2 = MPI_Wtime();

    if (process_rank == 0) {
        printf("PARRALEL TIME: %.5f\n", _time2-_time1);
        std::vector<int> multiply_on_vector;
        _time1 = MPI_Wtime();
        multiply_on_vector = multiplyOnVector(test_matrix, the_test_vector,
        matrix_count_rows, matrix_count_columns);
        _time2 = MPI_Wtime();
        printf("NOT PARRALEL TIME %.5f\n", _time2-_time1);
        ASSERT_EQ(multiply_on_vector, parallel_multiply_on_vector);
    }
}

TEST(Ribbon_horizontal_scheme, MatrixTest_21_35) {
    int process_rank;
    int matrix_count_rows = 21, matrix_count_columns = 35;
    std::vector<int> test_matrix(matrix_count_rows * matrix_count_columns);

    std::vector<int> the_test_vector(matrix_count_columns);

    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    if (process_rank == 0) {
       test_matrix = getRandomVector(matrix_count_rows, matrix_count_columns);
       the_test_vector = getRandomVector(matrix_count_columns, 1);
    }

    std::vector<int> parallel_multiply_on_vector;

    double _time1 = MPI_Wtime();
    parallel_multiply_on_vector = parallelMultiplyOnVector(
                test_matrix, the_test_vector, matrix_count_rows, matrix_count_columns);
    double _time2 = MPI_Wtime();

    if (process_rank == 0) {
        printf("PARRALEL TIME: %.5f\n", _time2-_time1);
        std::vector<int> multiply_on_vector;
        _time1 = MPI_Wtime();
        multiply_on_vector = multiplyOnVector(test_matrix, the_test_vector,
        matrix_count_rows, matrix_count_columns);
        _time2 = MPI_Wtime();
        printf("NOT PARRALEL TIME %.5f\n", _time2-_time1);
        ASSERT_EQ(multiply_on_vector, parallel_multiply_on_vector);
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
