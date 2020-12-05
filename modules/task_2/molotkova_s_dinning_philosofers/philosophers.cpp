// Copyright 2020 Molotkova Svetlana
#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <random>
#include <ctime>
#include <numeric>
#include <list>
#include<chrono>
#include<thread>
#include "../../../modules/task_2/molotkova_s_dinning_philosofers/Dinning_philosophers.h"


#define FORK_REQUEST 1
#define FORK_GET 2
#define FORK_SEND 3
#define ATE 4

void table(bool* dinner, bool* fork, int ProcSize, std::list<int> ph_queue) {
    MPI_Status msg;
    bool taken = false;
    int in_buffer, out_buffer;
    bool free = true;
    bool hungry_ph = false;
    bool fed_ph = true;
    for (int i = 0; i < ProcSize - 1; i++) {
        dinner[i] = hungry_ph;
        fork[i] = free;
    }
    dinner[0] = fed_ph;
    bool check = false;
    while (check != true) {
        MPI_Recv(&in_buffer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &msg);
        int philosopher = msg.MPI_SOURCE;

        if (msg.MPI_TAG == FORK_REQUEST) {
            if (fork[philosopher % (ProcSize - 1)] == free && fork[philosopher - 1] == free) {
                fork[philosopher % (ProcSize - 1)] = taken;
                fork[philosopher - 1] = taken;
                MPI_Send(&out_buffer, 1, MPI_INT, philosopher, FORK_GET, MPI_COMM_WORLD);
            } else {
                ph_queue.push_back(philosopher);
            }
        }
        if (msg.MPI_TAG == FORK_SEND) {
            fork[philosopher % (ProcSize - 1)] = free;
            fork[philosopher - 1] = free;
            dinner[philosopher] = fed_ph;

            for (size_t i = 0U; i < ph_queue.size(); ++i) {
                philosopher = ph_queue.front();
                if (fork[philosopher % (ProcSize - 1)] == free && fork[philosopher - 1] == free) {
                    fork[philosopher % (ProcSize - 1)] = taken;
                    fork[philosopher - 1] = taken;
                    MPI_Send(&out_buffer, 1, MPI_INT, philosopher, FORK_GET, MPI_COMM_WORLD);
                    ph_queue.remove(philosopher);
                }
            }
        }
        check = check_hunger(ProcSize, dinner, out_buffer);
    }
}

bool check_hunger(int ProcSize, bool* dinner, int out_buffer) {
    int check_hunger = 0;
    bool fed_ph = true;
    for (int i = 0; i < ProcSize; i++) {
        if (dinner[i] == fed_ph)
            check_hunger++;
    }
    if (check_hunger == ProcSize) {
        for (int i = 1; i < ProcSize; i++) {
            MPI_Send(&out_buffer, 1, MPI_INT, i, ATE, MPI_COMM_WORLD);
        }
        return true;
    }
    return false;
}

void philosofers(int time_) {
    MPI_Status msg;
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    bool check = false;
    int in_buffer, out_buffer;
    while (check != true) {
        std::this_thread::sleep_for(std::chrono::seconds(time_));
        MPI_Send(&out_buffer, 1, MPI_INT, 0, FORK_REQUEST, MPI_COMM_WORLD);
        MPI_Recv(&in_buffer, 1, MPI_INT, 0, FORK_GET, MPI_COMM_WORLD, &msg);
        std::this_thread::sleep_for(std::chrono::seconds(time_));
        MPI_Send(&out_buffer, 1, MPI_INT, 0, FORK_SEND, MPI_COMM_WORLD);
        MPI_Recv(&in_buffer, 1, MPI_INT, 0, ATE, MPI_COMM_WORLD, &msg);
        if (msg.MPI_TAG == ATE) {
            check = true;
        }
    }
}

void diningPhilosofers(int myRank, int ProcSize, int time) {
    if (ProcSize != 1) {
        std::list<int> ph_queue;
        bool* dinner = new bool[ProcSize - 1];
        bool* fork = new bool[ProcSize - 1];
        if (myRank == 0) {
            table(dinner, fork, ProcSize, ph_queue);
        } else {
            philosofers(time);
        }
    }
}
