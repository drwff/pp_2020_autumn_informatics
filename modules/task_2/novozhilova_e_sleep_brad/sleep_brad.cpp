// Copyright 2020 Novozhilova Ekaterina
#include <mpi.h>
#include <vector>
#include <iostream>
#include "../../../modules/task_2/novozhilova_e_sleep_brad/sleep_brad.h"

int Barbershop(const int q_max_size) {
    int curr_cl;
    int request = 0;
    int happy_cl = 4,
        disappointed_cl = 3,
        waiting_cl = 2;
    int busy_state = 1,
        sleep_state = 0;
    int barb_state = sleep_state;
    int Comm_rank, Comm_size;
    MPI_Comm_size(MPI_COMM_WORLD, &Comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &Comm_rank);
    // TAGS
    int barb_req = 1,
        happy_cl_tag = 2,
        curr_cl_tag = 3,
        ask_for_cl = 4,
        cancel_tag = 5;
        if (Comm_rank == 0) {
            std::vector<int> v(Comm_size - 2);
            std::vector<int> queue;
            int q_real_size = 0;
            MPI_Status status;
            int cl_count = 0;
            while (cl_count != Comm_size - 2) {
                MPI_Recv(&request, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                if (status.MPI_TAG == ask_for_cl) {
                    if (queue.empty()) {
                        // std::cout << "...Waiting for a client..." << std::endl;
                    } else {
                        curr_cl = queue.back();
                        queue.pop_back();
                        q_real_size--;
                        MPI_Send(&curr_cl, 1, MPI_INT, 1, curr_cl_tag, MPI_COMM_WORLD);
                        barb_state = busy_state;
                        std::cout << "rank no. " << curr_cl << " got a haircut and left" << std::endl;
                    }
                    continue;
                }
                if (status.MPI_TAG == happy_cl_tag) {
                    v[request - 2] = happy_cl;
                    barb_state = sleep_state;
                    cl_count++;
                    continue;
                }
                if (status.MPI_TAG == barb_req) {
                    if (barb_state == 1) {
                        if (q_real_size == q_max_size) {
                            std::cout << "rank no. " << status.MPI_SOURCE << " left" << std::endl;
                            v[status.MPI_SOURCE - 2] = disappointed_cl;
                            cl_count++;
                        }
                        if (q_real_size < q_max_size) {
                            queue.insert(queue.begin(), status.MPI_SOURCE);
                            v[status.MPI_SOURCE - 2] = waiting_cl;
                            q_real_size++;
                            std::cout << "rank no. " << status.MPI_SOURCE << " got in line" << std::endl;
                        }
                        continue;
                    }
                    if (barb_state == 0) {
                        if (queue.empty()) {
                            curr_cl = status.MPI_SOURCE;
                            MPI_Send(&curr_cl, 1, MPI_INT, 1, curr_cl_tag, MPI_COMM_WORLD);
                            barb_state = busy_state;
                            std::cout << "rank no. " << curr_cl << " got a haircut and left" << std::endl;
                        }
                        continue;
                    }
                }
            }
            MPI_Send(&request, 1, MPI_INT, 1, cancel_tag, MPI_COMM_WORLD);
            for (int i = 0; i < Comm_size - 2; i++) {
                if ((v[i] > happy_cl) || (v[i] < disappointed_cl)) {
                    return 1;
                }
            }
        }
        if (Comm_rank == 1) {
            MPI_Status statusB;
            int cancel = 0;
            while (cancel != 1) {
                MPI_Send(&request, 1, MPI_INT, 0, ask_for_cl, MPI_COMM_WORLD);
                MPI_Recv(&curr_cl, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &statusB);
                if (statusB.MPI_TAG == curr_cl_tag) {
                    MPI_Send(&curr_cl, 1, MPI_INT, 0, happy_cl_tag, MPI_COMM_WORLD);
                    continue;
                }
                if (statusB.MPI_TAG == cancel_tag) {
                    cancel = 1;
                }
            }
        }
        if ((Comm_rank != 0) && (Comm_rank != 1)) {
            MPI_Send(&request, 1, MPI_INT, 0, barb_req, MPI_COMM_WORLD);
        }
        return 0;
}
