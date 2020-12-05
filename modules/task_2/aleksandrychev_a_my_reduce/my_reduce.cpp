// Copyright 2020 Aleksandychev Andrey
#include <mpi.h>
#include <memory>
#include <cstring>
#include <iostream>
#include "../../../modules/task_2/aleksandrychev_a_my_reduce/my_reduce.h"

int my_reduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm) {
    if (sendbuf == nullptr || recvbuf == nullptr)
        return MPI_ERR_BUFFER;
    if (count <= 0)
        return MPI_ERR_COUNT;
    if (op != MPI_MAX && op != MPI_MIN && op != MPI_SUM && op != MPI_PROD)
        return MPI_ERR_OP;
    if (comm != MPI_COMM_WORLD && comm != MPI_COMM_SELF)
        return MPI_ERR_COMM;

    int size, rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    void* buffer;

    if (datatype == MPI_INT) {
        buffer = new int[count];
    } else if (datatype == MPI_FLOAT) {
        buffer = new float[count];
    } else if (datatype == MPI_DOUBLE) {
        buffer = new double[count];
    } else {
        return MPI_ERR_TYPE;
    }

    if (rank != root) {
        MPI_Send(sendbuf, count, datatype, root, 0, comm);
    }

    if (rank == root) {
        for (int proc = 0; proc < size; proc++) {
            if (proc == root) {
                if (datatype == MPI_INT) {
                    if (op == MPI_MAX) {
                        for (int j = 0; j < count; ++j) {
                            if (static_cast<int *>(recvbuf)[j] < static_cast<int *>(sendbuf)[j])
                                static_cast<int *>(recvbuf)[j] = static_cast<int *>(sendbuf)[j];
                        }
                    } else if (op == MPI_MIN) {
                        for (int j = 0; j < count; ++j) {
                            if (static_cast<int *>(recvbuf)[j] > static_cast<int *>(sendbuf)[j])
                                static_cast<int *>(recvbuf)[j] = static_cast<int *>(sendbuf)[j];
                        }
                    } else if (op == MPI_SUM) {
                        for (int j = 0; j < count; ++j) {
                            static_cast<int*>(recvbuf)[j] += static_cast<int*>(sendbuf)[j];
                        }
                    } else if (op == MPI_PROD) {
                        for (int j = 0; j < count; ++j) {
                            static_cast<int *>(recvbuf)[j] *= static_cast<int *>(sendbuf)[j];
                        }
                    }
                } else if (datatype == MPI_FLOAT) {
                    if (op == MPI_MAX) {
                        for (int j = 0; j < count; ++j) {
                            if (static_cast<float *>(recvbuf)[j] < static_cast<float *>(sendbuf)[j])
                                static_cast<float *>(recvbuf)[j] = static_cast<float *>(sendbuf)[j];
                        }
                    } else if (op == MPI_MIN) {
                        for (int j = 0; j < count; ++j) {
                            if (static_cast<float *>(recvbuf)[j] > static_cast<float *>(sendbuf)[j])
                                static_cast<float *>(recvbuf)[j] = static_cast<float *>(sendbuf)[j];
                        }
                    } else if (op == MPI_SUM) {
                        for (int j = 0; j < count; ++j) {
                            static_cast<float *>(recvbuf)[j] += static_cast<float *>(sendbuf)[j];
                        }
                    } else if (op == MPI_PROD) {
                        for (int j = 0; j < count; ++j) {
                            static_cast<float *>(recvbuf)[j] *= static_cast<float *>(sendbuf)[j];
                        }
                    }
                } else if (datatype == MPI_DOUBLE) {
                    if (op == MPI_MAX) {
                        for (int j = 0; j < count; ++j) {
                            if (static_cast<double *>(recvbuf)[j] < static_cast<double *>(sendbuf)[j])
                                static_cast<double *>(recvbuf)[j] = static_cast<double *>(sendbuf)[j];
                        }
                    } else if (op == MPI_MIN) {
                        for (int j = 0; j < count; ++j) {
                            if (static_cast<double *>(recvbuf)[j] > static_cast<double *>(sendbuf)[j])
                                static_cast<double *>(recvbuf)[j] = static_cast<double *>(sendbuf)[j];
                        }
                    } else if (op == MPI_SUM) {
                        for (int j = 0; j < count; ++j) {
                            static_cast<double *>(recvbuf)[j] += static_cast<double *>(sendbuf)[j];
                        }
                    } else if (op == MPI_PROD) {
                        for (int j = 0; j < count; ++j) {
                            static_cast<double *>(recvbuf)[j] *= static_cast<double *>(sendbuf)[j];
                        }
                    }
                }
                continue;
            }

            MPI_Status status;
            MPI_Recv(buffer, count, datatype, proc, 0, comm, &status);
            if (datatype == MPI_INT) {
                if (op == MPI_MAX) {
                    for (int j = 0; j < count; ++j) {
                        if (static_cast<int *>(recvbuf)[j] < static_cast<int *>(buffer)[j])
                            static_cast<int *>(recvbuf)[j] = static_cast<int *>(buffer)[j];
                    }
                } else if (op == MPI_MIN) {
                    for (int j = 0; j < count; ++j) {
                        if (static_cast<int *>(recvbuf)[j] > static_cast<int *>(buffer)[j])
                            static_cast<int *>(recvbuf)[j] = static_cast<int *>(buffer)[j];
                    }
                } else if (op == MPI_SUM) {
                    for (int j = 0; j < count; ++j) {
                        static_cast<int*>(recvbuf)[j] += static_cast<int*>(buffer)[j];
                    }
                } else if (op == MPI_PROD) {
                    for (int j = 0; j < count; ++j) {
                        static_cast<int *>(recvbuf)[j] *= static_cast<int *>(buffer)[j];
                    }
                }
            } else if (datatype == MPI_FLOAT) {
                if (op == MPI_MAX) {
                    for (int j = 0; j < count; ++j) {
                        if (static_cast<float *>(recvbuf)[j] < static_cast<float *>(buffer)[j])
                            static_cast<float *>(recvbuf)[j] = static_cast<float *>(buffer)[j];
                    }
                } else if (op == MPI_MIN) {
                    for (int j = 0; j < count; ++j) {
                        if (static_cast<float *>(recvbuf)[j] > static_cast<float *>(buffer)[j])
                            static_cast<float *>(recvbuf)[j] = static_cast<float *>(buffer)[j];
                    }
                } else if (op == MPI_SUM) {
                    for (int j = 0; j < count; ++j) {
                        static_cast<float *>(recvbuf)[j] += static_cast<float *>(buffer)[j];
                    }
                } else if (op == MPI_PROD) {
                    for (int j = 0; j < count; ++j) {
                        static_cast<float *>(recvbuf)[j] *= static_cast<float *>(buffer)[j];
                    }
                }
            } else if (datatype == MPI_DOUBLE) {
                if (op == MPI_MAX) {
                    for (int j = 0; j < count; ++j) {
                        if (static_cast<double *>(recvbuf)[j] < static_cast<double *>(buffer)[j])
                            static_cast<double *>(recvbuf)[j] = static_cast<double *>(buffer)[j];
                    }
                } else if (op == MPI_MIN) {
                    for (int j = 0; j < count; ++j) {
                        if (static_cast<double *>(recvbuf)[j] > static_cast<double *>(buffer)[j])
                            static_cast<double *>(recvbuf)[j] = static_cast<double *>(buffer)[j];
                    }
                } else if (op == MPI_SUM) {
                    for (int j = 0; j < count; ++j) {
                        static_cast<double *>(recvbuf)[j] += static_cast<double *>(buffer)[j];
                    }
                } else if (op == MPI_PROD) {
                    for (int j = 0; j < count; ++j) {
                        static_cast<double *>(recvbuf)[j] *= static_cast<double *>(buffer)[j];
                    }
                }
            }
        }
    }

    if (datatype == MPI_INT)
        delete[] static_cast<int*>(buffer);
    else if (datatype == MPI_FLOAT)
        delete[] static_cast<float*>(buffer);
    else if (datatype == MPI_DOUBLE)
        delete[] static_cast<double*>(buffer);

    return MPI_SUCCESS;
}
