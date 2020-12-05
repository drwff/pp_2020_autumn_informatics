// Copyright 2020 Kiseleva Anastasia
#include "../../../modules/task_2/kiseleva_gaus_zhordan/zhordan_gauss.h"
#include <mpi.h>
#include <ctime>
#include <cmath>
#include <limits>
#include <iostream>


void str_del_temp(double* array, int stlb, double temp, int j) {
    if (temp != 0) {
        for (int i = j*stlb; i < stlb+j*stlb; i++) {
            if (array[i] != 0) {
                array[i] = array[i] / temp;
            }
        }
    }
}

void str_umn_temp(double* array, int stlb, double temp, int j) {
    for (int i = j*stlb; i < stlb* (j+1); i++) {
        array[i] = array[i] * temp;
    }
}

void str_minus_str(double* array1, double* array2, int stlb, int j) {
    int p = 0;
    for (int i = j*stlb; i < stlb*(j+1); i++) {
        array1[i] = array1[i] - array2[p];
        p++;
    }
}

bool Check(double* array, int str, double* ans, double epsilon) {
    int stlb = str + 1;
    double tmp_sum = 0.0;
    int id;
    int id_ans;
    for (int row = 0; row <str; row++) {
        id_ans = 0;
        for (int col = 0; col < stlb - 1; col++) {
            tmp_sum += (array[row * stlb + col] * ans[id_ans]);
            id = row * stlb + col;
            id_ans++;
        }
        if ((tmp_sum - array[id + 1] >= epsilon) || (tmp_sum - array[id + 1] <= -epsilon)) {
            return false;
        }
        tmp_sum = 0.0;
    }
    return true;
}

double* matr(double* array, int str) {
    int stlb = str + 1;
    double k = 2.0;
    for (int i = 0; i < str; i++) {
        for (int j = 0; j < stlb - 1; j++) {
            if (i == j) continue;
            array[i * stlb + j] = k;
            k += 1.0;
        }
    }
    for (int i = 0; i < str; i++) {
        array[i * stlb + i] = static_cast<double>(i) + 1.0;
    }
    double sum = 0.0;
    for (int i = 0; i < str; i++) {
        for (int j = 0; j < stlb - 1; j++) {
            sum += array[i * stlb + j];
        }
        array[i * stlb + str] = sum;
        sum = 0;
    }
    return array;
}

double*posled(double* array, int str) {
    int step = 0;
    int stlb = str + 1;
    double* local_array2 = new double[stlb];
    double* local_array22 = new double[stlb];
    double* res = new double[str];
    while (step < str) {
        str_del_temp(array, stlb, array[(step*(stlb + 1))], step);
        int p = 0;
        for (int j = stlb * step; j < stlb*(step + 1); j++) {
            local_array2[p] = array[j];
            p++;
        }
        for (int j = 0; j < stlb; j++) {
            local_array22[j] = local_array2[j];
        }
        for (int k = 0; k < str; k++) {
            if (k != step) {
                for (int j = 0; j < stlb; j++) {
                    local_array2[j] = local_array22[j];
                }
                str_umn_temp(local_array2, stlb, array[(step + (stlb * k))], 0);
                str_minus_str(array, local_array2, stlb, k);
            }
        }
        step++;
    }
    for (int i = 0; i < str; i++) {
        res[i] = array[i * (stlb + 1)];
    }
    return res;
}

double* zhordan_gauss(double* array, int str) {
    int stlb = str + 1;
    int size, rank;
    double res_tmp;
    double* res = new double[str];
    int step = 0;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size > str-1) {
        double* local_array = new double[stlb];
        double* local_array2 = new double[stlb];
        MPI_Comm comm_world;
        MPI_Group group_world;
        comm_world = MPI_COMM_WORLD;
        MPI_Comm_group(comm_world, &group_world);
        int *members = new int[str];
        for (int i = 0; i < str; i++) {
            members[i] = i;
        }
        MPI_Group newworld;
        MPI_Group_incl(group_world, str, members, &newworld);
        MPI_Comm comm_newworld;
        MPI_Comm_create(comm_world, newworld, &comm_newworld);
        MPI_Group_rank(newworld, &rank);
        int size_;
        MPI_Group_size(newworld, &size_);
        if (comm_newworld != MPI_COMM_NULL) {
                if (rank == 0) {
                    for (int i = 1; i < size_; i++) {
                        MPI_Send(&array[0] + stlb * i, stlb, MPI_DOUBLE, i, 0, comm_newworld);
                    }
                    for (int i = 0; i < stlb; i++) {
                        local_array[i] = array[i];
                    }
                } else {
                    MPI_Recv(&local_array[0], stlb, MPI_DOUBLE, 0, 0, comm_newworld, &status);
                }
            MPI_Barrier(comm_newworld);
            while (step < str) {
                MPI_Barrier(comm_newworld);
                for (int i = 0; i < size_; i++) {
                    if (rank == i) {
                        str_del_temp(local_array, stlb, local_array[step], 0);
                        for (int j = 0; j < stlb; j++) {
                            local_array2[j] = local_array[j];
                        }
                        MPI_Bcast(&local_array2[0], stlb, MPI_DOUBLE, i, comm_newworld);
                    } else {
                        MPI_Bcast(&local_array2[0], stlb, MPI_DOUBLE, i, comm_newworld);
                        str_umn_temp(local_array2, stlb, local_array[step], 0);
                        str_minus_str(local_array, local_array2, stlb, 0);
                    }
                    step++;
                }
            }
            if (step == str) {
                for (int i = 0; i < str; i++) {
                    if (rank == i) {
                        res_tmp = local_array[step];
                    }
                }
            }
            MPI_Gather(&res_tmp, 1, MPI_DOUBLE, &res[0], 1, MPI_DOUBLE, 0, comm_newworld);
        }
    } else {
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        int delta = str / size;
        int ost = str - delta * size;
        double* local_array1;
        double* local_array2 = new double[stlb];
        double* local_array22 = new double[stlb];
            if (rank == 0) {
                local_array1 = new double[stlb*(delta + ost)];
                for (int i = 0; i < size-1; i++) {
                    MPI_Send(&array[((delta + ost)*stlb)] + stlb * i*delta, stlb*delta,
                        MPI_DOUBLE, (i + 1), 3, MPI_COMM_WORLD);
                }
                for (int i = 0; i < stlb*(delta + ost); i++) {
                    local_array1[i] = array[i];
                }
            } else {
                local_array1 = new double[stlb*delta];
                MPI_Recv(&local_array1[0], stlb*delta, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD, &status);
            }
        MPI_Barrier(MPI_COMM_WORLD);
        while (step < delta + ost) {
            if (rank == 0) {
                str_del_temp(local_array1, stlb, local_array1[(step*(stlb+1))], step);
                int p = 0;
                for (int j = stlb * step; j < stlb*(step + 1); j++) {
                    local_array2[p] = local_array1[j];
                    p++;
                }
                for (int j = 0; j < stlb; j++) {
                    local_array22[j] = local_array2[j];
                }
                for (int j = 1; j < size; j++) {
                    MPI_Send(&local_array2[0], stlb, MPI_DOUBLE, j, 1, MPI_COMM_WORLD);
                }
            } else {
                MPI_Recv(&local_array2[0], stlb, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
                for (int j = 0; j < stlb; j++) {
                    local_array22[j] = local_array2[j];
                }
            }
            if (rank == 0) {
                for (int k = 0; k < (delta + ost); k++) {
                    if (k != step) {
                        for (int j = 0; j < stlb; j++) {
                            local_array2[j] = local_array22[j];
                        }
                        str_umn_temp(local_array2, stlb, local_array1[(step + (stlb * k))], 0);
                        str_minus_str(local_array1, local_array2, stlb, k);
                    }
                }
            } else {
                for (int k = 0; k < delta; k++) {
                    for (int g = 0; g < stlb; g++) {
                        local_array2[g] = local_array22[g];
                    }
                    str_umn_temp(local_array2, stlb, local_array1[step+stlb*k], 0);
                    str_minus_str(local_array1, local_array2, stlb, k);
                }
            }
            MPI_Barrier(MPI_COMM_WORLD);
            step++;
        }
        MPI_Barrier(MPI_COMM_WORLD);
        int smesh;
            smesh = 0;
        int o = 0;
        while (step <str) {
           for (int i = 1; i < size; i++) {
               int z = 0;
               while (z < delta) {
                   if (rank == i) {
                       str_del_temp(local_array1, stlb, local_array1[step + stlb * smesh], smesh);
                       int p = 0;
                       for (int j = stlb * o; j < stlb*(o + 1); j++) {
                           local_array2[p] = local_array1[j];
                           p++;
                       }
                       for (int j = 0; j < stlb; j++) {
                           local_array22[j] = local_array2[j];
                           p++;
                       }
                       for (int j = 0; j < size; j++) {
                           if (j != i) {
                               MPI_Send(&local_array2[0], stlb, MPI_DOUBLE, j, 2, MPI_COMM_WORLD);
                           }
                       }
                       for (int k = 0; k < delta; k++) {
                           if (k != o) {
                               str_umn_temp(local_array2, stlb, local_array1[step + stlb * k], 0);
                               str_minus_str(local_array1, local_array2, stlb, k);
                               for (int j = 0; j < stlb; j++) {
                                   local_array2[j] = local_array22[j];
                               }
                           }
                       }
                       smesh++;
                       o++;
                   }
               if (rank != i) {
                   MPI_Recv(&local_array2[0], stlb, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, &status);
                   for (int j = 0; j < stlb; j++) {
                       local_array22[j] = local_array2[j];
                   }
                   if (rank == 0) {
                       for (int k = 0; k < delta + ost; k++) {
                           for (int j = 0; j < stlb; j++) {
                               local_array2[j] = local_array22[j];
                           }
                           str_umn_temp(local_array2, stlb, local_array1[step + stlb * k], 0);
                           str_minus_str(local_array1, local_array2, stlb, k);
                       }
                   } else {
                       for (int k = 0; k < delta; k++) {
                           if (k != step) {
                               for (int j = 0; j < stlb; j++) {
                                   local_array2[j] = local_array22[j];
                               }
                               str_umn_temp(local_array2, stlb, local_array1[step + stlb * k], 0);
                               str_minus_str(local_array1, local_array2, stlb, k);
                           }
                       }
                   }
               }
               step++;
               z++;
               }
            }
            MPI_Barrier(MPI_COMM_WORLD);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        double *res_;
        res_ = new double[delta];
        if (step == str) {
            if (rank != 0) {
                for (int i = 0 ; i < delta; i++) {
                    res_[i] = local_array1[str + (stlb * i)];
                }
                for (int i = 1; i < size; i++) {
                    if (rank == i) {
                        MPI_Send(&res_[0], delta, MPI_DOUBLE, 0, i, MPI_COMM_WORLD);
                    }
                }
            }
            if (rank == 0) {
                for (int i = 1; i < size; i++) {
                    MPI_Recv(&res[delta*i + ost], delta, MPI_DOUBLE, i, i, MPI_COMM_WORLD, &status);
                }
                for (int h = 0; h < (delta + ost); h++) {
                    res[h] = local_array1[str + stlb * h];
                }
            }
        }
    }
    return res;
}
