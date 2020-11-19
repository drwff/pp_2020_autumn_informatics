// Copyright 2020 Tyurmina Alexandra

#include <mpi.h>
#include <string>

#include "../../../modules/task_1/tyurmina_a_word_count/word_count.h"

int string_not_correct(std::string s) {
    if (s.empty()) {
        return 0;
    } else {
        return 1;
    }
}

int two_space(std::string s) {
    for (int i = 0; i < static_cast <int>(s.length()) - 1; i++) {
        if ((s[i] == ' ') && (s[i + 1] == ' ')) return 0;
    }
    return 1;
}
/*std::string random_str(int size) {
    int k = 1;
    std::string s;
    char words[] = { 'wow','cat','dog','one','pet','and','bad','win','with','Jack','Ammy' };
    s += words[rand() % 10];
    while (k < size) {
        s += ' ';
        s += words[rand() % 10];
        k++;
    }
    return s;
}*/

int count_words(std::string s, int z) {
    int count = 0;
    int len = s.length();
    for (int i = 0; i < len; i++) {
        if (s[i] == ' ') {
            count++;
        }
    }
    count += z;
    return count;
}

int parallel_count_words(std::string s, int z) {
    int size = 0, rank = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int part, part2;
    part = s.length() / size;
    part2 = s.length() % size;

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            int a = part2 + part * i;
            MPI_Send(&a, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Bcast(&part, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int count1;
    int countres = 0;
    std::string s1 = "";
    int a;

    if (rank == 0) {
        s1 = s.substr(0, part2 + part);
    } else {
        MPI_Status status;
        MPI_Recv(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        for (int i = a+1; i <= a+part; i++) {
            s1 += s[i];
        }
    }
    count1 = count_words(s1, 0);
    MPI_Reduce(&count1, &countres, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    countres++;
    return countres++;
}
