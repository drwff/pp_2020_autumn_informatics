// Copyright 2020 Pestreev Daniil
#include <mpi.h>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <random>
#include <vector>
#include "../../../modules/task_2/pestreev_d_reader_writer/reader_writer.h"

const int access_W = 100;
const int access_R = 200;
const int access_ReadFinish = 300;
const int iden_type_of_access = 9999;

int reader_writer(int writer_count, std::vector<int> data) {
    int proc_rank, proc_size;
    int ready_recv = 1;
    int access;
    int changedata = 0;
    int flagg = 0;
    int readercount = 0;
    int reading_processes = 0;
    int writing_processes = 0;
    int datasize = data.size();

    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Status statusMpi;
    MPI_Request requestMpi;

    if (writer_count < proc_size - 2 && writer_count > 0) {
        if (proc_rank == 0) {
            while (true) {
                if (ready_recv == 1) {
                    MPI_Irecv(&access, 1, MPI_INT, MPI_ANY_SOURCE, iden_type_of_access, MPI_COMM_WORLD, &requestMpi);
                    ready_recv = 0;
                }
                if (!ready_recv) {
                    MPI_Test(&requestMpi, &flagg, &statusMpi);
                    if (flagg && access == access_W) {
                        MPI_Send(&datasize, 1, MPI_INT,  statusMpi.MPI_SOURCE, 151, MPI_COMM_WORLD);
                        MPI_Send(&data[0], datasize, MPI_INT, statusMpi.MPI_SOURCE, access_W, MPI_COMM_WORLD);
                        std::cout <<"-------------------------------------"<< std::endl;
                        std::cout << "Process " << statusMpi.MPI_SOURCE << " is writing:" << std::endl;
                        MPI_Recv(&changedata, 1, MPI_INT, MPI_ANY_SOURCE, 66, MPI_COMM_WORLD, &statusMpi);
                        if (changedata) {
                            int i;
                            MPI_Recv(&i, 1, MPI_INT, MPI_ANY_SOURCE, 8, MPI_COMM_WORLD, &statusMpi);
                            datasize = i;
                        }
                        data.resize(datasize);
                        int d = datasize;
                        MPI_Status s = statusMpi;
                        MPI_Recv(&data[0], d, MPI_INT, MPI_ANY_SOURCE, statusMpi.MPI_SOURCE, MPI_COMM_WORLD, &s);
                        std::cout << "data = " << std::endl;
                        for (int i = 0; i < datasize; i++) {
                           std::cout << data[i] << "|";
                        }
                        std::cout << std::endl;
                        ready_recv = 1;
                        writing_processes++;
                    }
                    if (flagg && access == access_R) {
                        readercount++;
                        std::cout <<"-------------------------------------"<< std::endl;
                        std::cout << "Process " << statusMpi.MPI_SOURCE << " is reading:" << std::endl;
                        std::cout << "data = " << std::endl;
                        MPI_Send(&datasize, 1, MPI_INT,  statusMpi.MPI_SOURCE, 151, MPI_COMM_WORLD);
                        for (int i = 0; i < datasize; i++) {
                            std::cout << data[i] << "|";
                        }
                        std::cout << std::endl;
                        std::cout << "Readers count: " << readercount << std::endl;
                        data.resize(datasize);
                        int d = datasize;
                        MPI_Request r = requestMpi;
                        MPI_Isend(&data[0], d, MPI_INT, statusMpi.MPI_SOURCE, access_R, MPI_COMM_WORLD, &r);
                        ++reading_processes;
                        ready_recv = 1;
                    }
                    if (flagg && access == access_ReadFinish) {
                        readercount--;
                        std::cout << "Process " << statusMpi.MPI_SOURCE << " finish reading:" << std::endl;
                        std::cout << "Readers count: " << readercount << std::endl;
                        ready_recv = 1;
                    }
                }

                if (readercount == 0) {
                    if (reading_processes == proc_size - writer_count) {
                        if (writing_processes == (writer_count - 1)) {
                            break;
                        }
                    }
                }
            }
        }

        if (proc_rank > 0 && proc_rank < writer_count) {  // writer
            access = access_W;
            MPI_Send(&access, 1, MPI_INT, 0, iden_type_of_access, MPI_COMM_WORLD);
            MPI_Recv(&datasize, 1, MPI_INT, 0, 151, MPI_COMM_WORLD, &statusMpi);             //
            data.resize(datasize);                                                           //
            MPI_Recv(&data[0], datasize, MPI_INT, 0, access_W, MPI_COMM_WORLD, &statusMpi);  // getting data

            data.push_back(proc_rank);  // modificate data
            int comp = data.size();
            if (datasize != comp) {
                changedata = 1;
            } else {
                changedata = 0;
            }
            int i = data.size();
            MPI_Send(&changedata, 1, MPI_INT, 0, 66, MPI_COMM_WORLD);
            // if modified datasize, then send size of recv buffer
            if (changedata) {
                MPI_Send(&i, 1, MPI_INT, 0, 8, MPI_COMM_WORLD);
            }
            MPI_Send(&data[0], data.size(), MPI_INT, 0, proc_rank, MPI_COMM_WORLD);  // send modified data
        }

        if (proc_rank >= writer_count && proc_rank < proc_size) {  // reader
            access = access_R;
            MPI_Send(&access, 1, MPI_INT, 0, iden_type_of_access, MPI_COMM_WORLD);
            //
            MPI_Recv(&datasize, 1, MPI_INT, 0, 151, MPI_COMM_WORLD, &statusMpi);
            data.resize(datasize);
            //
            MPI_Recv(&data[0], datasize, MPI_INT, 0, access_R, MPI_COMM_WORLD, &statusMpi);
            //
            access = access_ReadFinish;
            MPI_Send(&access, 1, MPI_INT, 0, iden_type_of_access, MPI_COMM_WORLD);
        }
    } else {
        throw -1;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    return datasize;
}
