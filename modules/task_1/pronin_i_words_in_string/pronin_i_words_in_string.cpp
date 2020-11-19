// Copyright 2020 Pronin Igor
#include <mpi.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <cstring>
#include "../../../modules/task_1/pronin_i_words_in_string/pronin_i_words_in_string.h"
std::string RandomString(int n) {
    if (n < 0)
        throw "Wrong size";
    srand((unsigned)time(NULL));
    const char s[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .!,?";
    const int strLen = strlen(s);
    std::string ss = "";
    for (int i = 0; i < n; i++)
        ss += s[rand() % strLen];
    return ss;
}
int SequentialOperations(std::string a) {
    int count = 0;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] == ' ')
            count++;
    }
    return (count + 1);
}
int ParallelOperations(std::string str, int len) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = len / size;
    const int epsilon = len - delta * size;
    if (rank == 0) {
        for (int i = 1; i < size; i++)
            MPI_Send(str.data() + epsilon + delta * i, delta, MPI_CHAR, i, 0, MPI_COMM_WORLD);
    }
    std::string localStr = "";
    if (rank == 0) {
        localStr = std::string(str.cbegin(), str.cbegin() + epsilon + delta);
    } else {
        std::vector <char> local(delta);
        MPI_Status status;
        MPI_Recv(&local[0], delta, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        localStr = std::string(local.cbegin(), local.cend());
    }
    int single = SequentialOperations(localStr);
    int common = 0;
    MPI_Reduce(&single, &common, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return (common - (size - 1));
}
