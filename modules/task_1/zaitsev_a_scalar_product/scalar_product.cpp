// Copyright 2020 Zaitsev Andrey
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include "../../../modules/task_1/zaitsev_a_scalar_product/scalar_product.h"

std::vector<int> createRandomVector(int size) {
  std::mt19937 gen;
  gen.seed(static_cast<int>(time(0)));
  std::vector<int> result(size);
  for (int& i : result)
    i = gen() % 100;
  return result;
}

int scalar_product_of_part_vectors(const std::vector<int>& vec1, const std::vector<int>& vec2) {
  int result = 0;

  for (int i = 0; i < static_cast<int>(vec1.size()); i++)
    result += vec1[i] * vec2[i];
  return result;
}

int scalar_product_parallel(const std::vector<int>& vec1, const std::vector<int>& vec2, int size) {
  if (size == 0)
    return 0;

  int process_size, process_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &process_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
  int bunch = size / process_size;
  int remain = size - bunch * process_size;
  if (bunch == 0) {
    if (process_rank == 0) {
      for (int i = 1; i < size; i++) {
        MPI_Send(&vec1[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        MPI_Send(&vec2[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      }
    }
  } else {
    if (process_rank == 0) {
      int tmp = 1;
      for (int process_num = 1; process_num < process_size; process_num++) {
        if (tmp != remain && remain != 0) {
          MPI_Send(&vec1[process_num - 1] + bunch * process_num + 1,
            bunch + 1, MPI_INT, process_num, 0, MPI_COMM_WORLD);
          MPI_Send(&vec2[process_num - 1] + bunch * process_num + 1,
            bunch + 1, MPI_INT, process_num, 0, MPI_COMM_WORLD);
          tmp++;
        } else {
          MPI_Send(&vec1[remain] + bunch * process_num, bunch, MPI_INT,
            process_num, 0, MPI_COMM_WORLD);
          MPI_Send(&vec2[remain] + bunch * process_num, bunch, MPI_INT,
            process_num, 0, MPI_COMM_WORLD);
        }
      }
    }
  }
  std::vector<int> part_vector1, part_vector2;
  if (bunch == 0) {
    part_vector1.resize(1);
    part_vector2.resize(1);
    if (process_rank == 0) {
      part_vector1 = { vec1.begin(), vec1.begin() + 1 };
      part_vector2 = { vec2.begin(), vec2.begin() + 1 };
    } else if (process_rank < size) {
      MPI_Status status;
      MPI_Recv(&part_vector1[0], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
      MPI_Recv(&part_vector2[0], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
  } else {
    if (process_rank == 0 && remain != 0) {
      part_vector1.resize(bunch + 1);
      part_vector2.resize(bunch + 1);
      part_vector1 = { vec1.begin(), vec1.begin() + bunch + 1 };
      part_vector2 = { vec2.begin(), vec2.begin() + bunch + 1 };
    } else if (process_rank == 0) {
      part_vector1.resize(bunch);
      part_vector2.resize(bunch);
      part_vector1 = { vec1.begin(), vec1.begin() + bunch };
      part_vector2 = { vec2.begin(), vec2.begin() + bunch };
    } else {
      if (1 <= process_rank && process_rank < remain) {
        part_vector1.resize(bunch + 1);
        part_vector2.resize(bunch + 1);
        MPI_Status status;
        MPI_Recv(&part_vector1[0], bunch + 1, MPI_INT, 0, 0,
          MPI_COMM_WORLD, &status);
        MPI_Recv(&part_vector2[0], bunch + 1, MPI_INT, 0, 0,
          MPI_COMM_WORLD, &status);
      } else {
        part_vector1.resize(bunch);
        part_vector2.resize(bunch);
        MPI_Status status;
        MPI_Recv(&part_vector1[0], bunch, MPI_INT, 0, 0,
          MPI_COMM_WORLD, &status);
        MPI_Recv(&part_vector2[0], bunch, MPI_INT, 0, 0,
          MPI_COMM_WORLD, &status);
      }
    }
  }

  int sum = 0;

  int part_sum = scalar_product_of_part_vectors(part_vector1, part_vector2);
  MPI_Reduce(&part_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  return sum;
}
