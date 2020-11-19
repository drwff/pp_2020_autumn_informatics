// Copyright 2020 Lobov Alexander
#include <mpi.h>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../modules/task_1/lobov_a_sorted_strings/sorted_strings.h"

std::string getRandomString(int size) {
    if (size < 0)
        throw "Incorrect size";

    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));

    std::string res;
    res.resize(size);
    for (int i = 0; i < size; i++) {
        res[i] = gen() % 256;
    }
    return res;
}

bool isSortedStringPair(std::string lhs, std::string rhs) {
    return !(lhs > rhs);
}

bool isSortedStringPairParallel(std::string lhs, std::string rhs) {
    if (lhs.size() < rhs.size())
        return true;
    if (lhs.size() > rhs.size())
        return false;

    int strLen = lhs.size();

    if (strLen == 0)
        return true;

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size > strLen) size = strLen;
    if (rank >= size) return false;
    const int delta_symbols = strLen / size;
    const int rem_symbols = strLen % size;

    int symbols_per_process = delta_symbols;
    if (rank == 0) {
        symbols_per_process = rem_symbols + delta_symbols;
    }

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(lhs.c_str() + rem_symbols + proc * delta_symbols, delta_symbols,
                MPI_CHAR, proc, 0, MPI_COMM_WORLD);
            MPI_Send(rhs.c_str() + rem_symbols + proc * delta_symbols, delta_symbols,
                MPI_CHAR, proc, 1, MPI_COMM_WORLD);
        }
    }

    std::string lhsLocalString;
    lhsLocalString.resize(symbols_per_process);
    std::string rhsLocalString;
    rhsLocalString.resize(symbols_per_process);

    if (rank == 0) {
        lhsLocalString = lhs.substr(0, symbols_per_process);
        rhsLocalString = rhs.substr(0, symbols_per_process);
    } else {
        MPI_Status status;
        MPI_Recv(&lhsLocalString[0], symbols_per_process,
            MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&rhsLocalString[0], symbols_per_process,
            MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
    }

    int localIsSorted = isSortedStringPair(lhsLocalString, rhsLocalString);

    if (rank != 0) {
        MPI_Send(&localIsSorted, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        return localIsSorted;
    }

    if (rank == 0) {
        if (!localIsSorted)
            return false;

        MPI_Status status;
        for (int proc = 1; proc < size; proc++) {
            int tmpIsSorted;
            MPI_Recv(&tmpIsSorted, 1, MPI_INT, proc, 0, MPI_COMM_WORLD, &status);
            if (!tmpIsSorted)
                return false;
        }
    }

    return true;
}
