// Copyright 2020 Gushchin Artem
#include <mpi.h>
#include <random>
#include <string>
#include <vector>
#include <algorithm>
#include "../../../modules/task_1/gushchin_a_lexicographic_order/lexicographic_order.h"

std::string getRandomString(size_t length) {
    std::random_device rd;
    std::mt19937 gen;
    gen.seed(rd());
    const std::string alphaNumeric("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    std::string randomString;
    randomString.reserve(length);
    for (size_t i = 0; i < length; i++)
        randomString.push_back(alphaNumeric[gen() % alphaNumeric.length()]);

    return randomString;
}

// 0 - string1 > string2, 1 - string1 < string2, 2 - string1 == string2
int sequentialIsLexicographicOrder(const std::string& string1, const std::string& string2) {
    if (string1 < string2)
        return 1;
    else if (string1 == string2)
        return 2;
    else
        return 0;
}

int parallelIsLexicographicOrder(const std::string& string1, const std::string& string2,
                                       size_t length1, size_t length2) {
    if (!length1 || !length2)
        throw "Lengths of strings should be greater than 0";

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    size_t minLength = std::min(length1, length2);

    if (static_cast<int>(minLength) < size)
        return sequentialIsLexicographicOrder(string1, string2);

    size_t delta = minLength / size;
    size_t reminder = minLength % size;

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(string1.c_str() + proc * delta + reminder, static_cast<int>(delta),
                     MPI_CHAR, proc, 0, MPI_COMM_WORLD);
            MPI_Send(string2.c_str() + proc * delta + reminder, static_cast<int>(delta),
                     MPI_CHAR, proc, 1, MPI_COMM_WORLD);
        }
    }

    std::string localString1;
    std::string localString2;

    if (rank == 0) {
        localString1 = string1.substr(0, delta + reminder);
        localString2 = string2.substr(0, delta + reminder);
    } else {
        std::vector<char> buffer(delta);
        localString1.resize(delta);
        localString1.resize(delta);

        MPI_Status status;
        MPI_Recv(&buffer[0], static_cast<int>(delta), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        localString1.assign(buffer.begin(), buffer.end());

        MPI_Recv(&buffer[0], static_cast<int>(delta), MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
        localString2.assign(buffer.begin(), buffer.end());
    }

    int localResult = sequentialIsLexicographicOrder(localString1, localString2);

    std::vector<int> results;

    if (rank == 0)
        results.resize(size);

    MPI_Gather(&localResult, 1, MPI_INT, &results[0], 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        bool result = false, equalBegginnings = true;

        for (const auto& locResult : results) {
            switch (locResult) {
            case 0:
                if (!result)
                    return 0;
                break;

            case 1:
                result = true;
                equalBegginnings = false;
                break;

            default:
                result = true;
                break;
            }
        }

        // 0 - string1 > string2, 1 - string1 < string2, 2 - string1 == string2
        if (equalBegginnings) {
            if (length1 > length2)
                return 0;
            else if (length1 == length2)
                return 2;
            else
                return 1;
        }

        return 1;
    }

    return localResult;
}
