// Copyright 2020 Makarov Alexander
#include <mpi.h>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/makarov_a_string_compare/string_compare.h"


std::string getRandomString(int sz) {
    if (sz <= 0) return std::string("");
    std::random_device rd;
    std::mt19937 gen(rd());
    std::string str(sz, 42);
    for (int i = 0; i < sz; i++)
        str[i] = 32 + gen() % 223;
    return str;
}

int compareSequential(const std::string& global_str1,
                      const std::string& global_str2) {
    const int sz1 = global_str1.size();
    const int sz2 = global_str2.size();
    for (int i = 0; i < std::min(sz1, sz2); i++) {
        char c1 = tolower(global_str1[i]), c2 = tolower(global_str2[i]);
        if (c1 > c2) return 1;
        if (c2 > c1) return 2;
    }
    if (sz1 > sz2) return 1;
    if (sz2 > sz1) return 2;
    return 0;
}

int compareParallel(const std::string& global_str1,
                    const std::string& global_str2,
                    const size_t str_size1, const size_t str_size2) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    size_t min_str_size = std::min(str_size1, str_size2);
    const size_t delta = min_str_size / size;
    const size_t remaind = min_str_size % size;

    if (str_size1 == 0 || str_size2 == 0) {
        if (str_size1 == 0 && str_size2 == 0) return 0;
        if (str_size1 == 0) return 2;
        if (str_size2 == 0) return 1;
    }

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(global_str1.data() + remaind + i * delta, delta,
                        MPI_CHAR, i, 0, MPI_COMM_WORLD);
            MPI_Send(global_str2.data() + remaind + i * delta, delta,
                        MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    }

    std::string local_str1, local_str2;
    if (rank == 0) {
        local_str1 = std::string(global_str1, 0, remaind + delta);
        local_str2 = std::string(global_str2, 0, remaind + delta);
    } else {
        std::vector<char> local_str_vec1(delta);
        std::vector<char> local_str_vec2(delta);
        MPI_Status status;
        MPI_Recv(local_str_vec1.data(), delta, MPI_CHAR, 0, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &status);
        MPI_Recv(local_str_vec2.data(), delta, MPI_CHAR, 0, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &status);
        local_str1 = std::string(local_str_vec1.cbegin(),
                                 local_str_vec1.cend());
        local_str2 = std::string(local_str_vec2.cbegin(),
                                 local_str_vec2.cend());
    }
    int local_result = compareSequential(local_str1, local_str2);
    if (rank == 0) {
        std::vector<int> results(size);
        results[0] = local_result;
        MPI_Status status;
        for (int i = 1; i < size; i++) {
            MPI_Recv(results.data() + i, 1, MPI_INT, i, MPI_ANY_TAG,
                     MPI_COMM_WORLD, &status);
        }
        for (int i = 0; i < size; i++)
            if (results[i] != 0) return results[i];
        if (str_size1 > str_size2) return 1;
        if (str_size2 > str_size1) return 2;
        return 0;
    } else {
        MPI_Send(&local_result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        return local_result;
    }
}
