// Copyright 2020 Lyutova Tanya
#include <mpi.h>
#include <iostream>
#include <queue>
#include <random>
#include "../../../modules/task_2/lyutova_t_sleeping_barber/sleeping_barber.h"

void wait(double seconds) {
    double time = MPI_Wtime();
    while (MPI_Wtime() - time < seconds) {
    }
}

void print(int id, const char* message, double number) {
    if (id == 0)
        std::cout << "BARBER: " << message << ' ' << number << std::endl;
    else
        std::cout << "CUSTOMER#" << id << ": " << message << ' ' << number << std::endl;
}

void print(int id, const char* message, int number) {
    if (id == 0)
        std::cout << "BARBER: " << message << ' ' << number << std::endl;
    else
        std::cout << "CUSTOMER#" << id << ": " << message << ' ' << number << std::endl;
}


void doBarber(int queue_size) {
    std::queue<int> queue;
    int customers_compl = 0;
    while (customers_compl != queue_size) {
        int is_waiting_customers = 0;
        int is_free = 0;  // is barber free
        // ask "if anybody waiting?"
        MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &is_waiting_customers, MPI_STATUS_IGNORE);
        int customer_id;
        while (is_waiting_customers) {
            MPI_Recv(&customer_id, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            print(0, "Hello, customer #", customer_id);
            if (static_cast<int>(queue.size()) < queue_size) {
                queue.push(customer_id);
                is_free = 1;
                MPI_Send(&is_free, 1, MPI_INT, customer_id, 0, MPI_COMM_WORLD);
            } else {
                is_free = 0;
                MPI_Send(&is_free, 1, MPI_INT, customer_id, 0, MPI_COMM_WORLD);
            }
            // ask "if anybody waiting?" again
            MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &is_waiting_customers, MPI_STATUS_IGNORE);
        }

        if (!queue.empty()) {
            customer_id = queue.front();
            queue.pop();
            print(0, "Start hair cutting for #", customer_id);
            int is_hair_cutting = 1;
            MPI_Send(&is_hair_cutting, 1, MPI_INT, customer_id, 0, MPI_COMM_WORLD);
            wait(0.5);
            print(0, "End hair cutting for #", customer_id);
            is_hair_cutting = 0;
            MPI_Send(&is_hair_cutting, 1, MPI_INT, customer_id, 0, MPI_COMM_WORLD);
            customers_compl++;
        }
    }
}

double getRandomTime() {
    std::random_device rd;
    std::mt19937 generator(rd());
    return (100.0 + static_cast<double>(generator() % 400u)) / 1000.0;
}

void doCustomer(int id) {
    double wait_time = getRandomTime();
    print(id, "Waiting on start...", wait_time);
    wait(wait_time);
    int is_barber_hair_cutting = 1;
    while (is_barber_hair_cutting != 0) {
        MPI_Send(&id, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        int is_barber_free = 0;
        MPI_Recv(&is_barber_free, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (is_barber_free) {
            MPI_Recv(&is_barber_free, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&is_barber_hair_cutting, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            double wait_time = getRandomTime();
            print(id, "Waiting for a barber...", wait_time);
            wait(wait_time);
        }
    }
}

void launch() {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (size < 2) {
        throw "Too little comm size";
    }
    if (rank == 0)
        doBarber(size - 1);
    else
        doCustomer(rank);
}
