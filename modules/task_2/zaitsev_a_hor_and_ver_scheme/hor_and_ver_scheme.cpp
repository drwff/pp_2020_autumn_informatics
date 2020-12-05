// Copyright 2020 Zaitsev Andrey
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <utility>
#include "../../../modules/task_2/zaitsev_a_hor_and_ver_scheme/hor_and_ver_scheme.h"

std::vector<double> createRandomMatrix(int size, time_t seed) {
  std::mt19937 gen(seed);
  std::vector<double> result(size * size);
  for (int i = 0; i < size * size; i++)
  result[i] = gen() % 50;
  return result;
}

std::vector<double> transposition(std::vector<double> matrix_B, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            std::swap(matrix_B[i * size + j], matrix_B[j * size + i]);
        }
    }
    return matrix_B;
}

double scalar_product_of_vectors(const std::vector<double>& vec1, const std::vector<double>& vec2) {
  double result = 0.0;
  for (int i = 0; i < static_cast<int>(vec1.size()); i++)
    result += vec1[i] * vec2[i];
  return result;
}

std::vector<double> matrix_product_sequential(const std::vector<double>& matrix_A,
  const std::vector<double>& matrix_B, int size) {
  std::vector<double> matrix_C(size * size);
  for (int i = 0; i < size; i++) {
    std::vector<double> row_A(matrix_A.begin() + i * size, matrix_A.begin() + i * size + size);
    for (int j = 0; j < size; j++) {
      std::vector<double> col_B(matrix_B.begin() + j * size, matrix_B.begin() + j * size + size);
      matrix_C[i * size + j] = scalar_product_of_vectors(row_A, col_B);
    }
  }
  return matrix_C;
}

std::vector<double> matrix_product_parallel(const std::vector<double>& matrix_A,
  const std::vector<double>& matrix_B, int size) {
    int process_size, process_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &process_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    int stripes = size / process_size;
    int part_elem_number = stripes * size;

    std::vector<double> matrix_C(size * size);
    std::vector<double> matrix_A_local(part_elem_number);
    std::vector<double> matrix_B_local(part_elem_number);
    std::vector<double> matrix_C_local(part_elem_number);

    MPI_Scatter(matrix_A.data(), part_elem_number, MPI_DOUBLE, matrix_A_local.data(),
    part_elem_number, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(matrix_B.data(), part_elem_number, MPI_DOUBLE, matrix_B_local.data(),
    part_elem_number, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  double tmp = 0.0;
    for (int i = 0; i < stripes; i++) {
        for (int j = 0; j < stripes; j++) {
            for (int k = 0; k < size; k++)
                tmp += matrix_A_local[i * size + k] * matrix_B_local[j * size + k];
      matrix_C_local[i * size + j + stripes * process_rank] = tmp;
            tmp = 0.0;
        }
    }

    int next_process, prev_process;
  int change;
  MPI_Status status;
    for (int p = 1; p < process_size; p++) {
    next_process = process_rank + 1;
    prev_process = process_rank - 1;

        if (process_rank == process_size - 1)
      next_process = 0;

        if (process_rank == 0)
      prev_process = process_size - 1;

        MPI_Sendrecv_replace(matrix_B_local.data(), part_elem_number, MPI_DOUBLE,
      next_process, 0, prev_process, 0, MPI_COMM_WORLD, &status);

        tmp = 0.0;
        for (int i = 0; i < stripes; i++) {
            for (int j = 0; j < stripes; j++) {
                for (int k = 0; k < size; k++)
                    tmp += matrix_A_local[i * size + k] * matrix_B_local[j * size + k];
                if (process_rank - p >= 0) {
          change = process_rank - p;
                } else {
          change = process_size - p + process_rank;
                }
        matrix_C_local[i * size + j + change * stripes] = tmp;
                tmp = 0.0;
            }
        }
    }

  MPI_Gather(matrix_C_local.data(), part_elem_number, MPI_DOUBLE, matrix_C.data(),
    part_elem_number, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (process_rank == 0) {
    int remain = size % process_size;
    if (remain != 0) {
      tmp = 0.0;
      for (int i = 0; i < stripes * process_size; i++) {
        for (int j = stripes * process_size; j < size; j++) {
          for (int k = 0; k < size; k++)
            tmp += matrix_A[i * size + k] * matrix_B[j * size + k];
          matrix_C[i * size + j] = tmp;
          tmp = 0.0;
        }
      }

      tmp = 0.0;
      for (int i = stripes * process_size; i < size; i++) {
        for (int j = 0; j < size; j++) {
          for (int k = 0; k < size; k++)
            tmp += matrix_A[i * size + k] * matrix_B[j * size + k];
          matrix_C[i * size + j] = tmp;
          tmp = 0.0;
        }
      }
    }
  }

    return matrix_C;
}
