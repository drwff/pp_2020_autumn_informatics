// Copyright 2020 Fedoseev Valera

#include <mpi.h>
#include <vector>
#include <ctime>
#include <random>
#include "../../../modules/task_1/fedoseev_v_matrix_line_max/MatirxLineMax.h"


std::vector<int> getRandomMatrixinVector(int rows, int columns) {
  if (rows <= 0 || columns <= 0) throw "Matrix is incrorrect";

  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> matrix(rows * columns);
  for (int i = 0; i < rows*columns; i++) { matrix[i] = gen() % 100; }
  return matrix;
}

std::vector<int> getLineMaxSequental(const std::vector<int>matrix, int rows, int columns) {
  if (rows <= 0 || columns <= 0) throw "Matrix is incrorrect";

  int inLineMax = 0;
  int n = 1;
  int j = 0;
  std::vector<int>result(rows);
  for (int i = 0; i < rows*columns; i++) {
  if (i + 1 == columns*n) {
  if (inLineMax < matrix[i]) inLineMax = matrix[i];
  result[j] = inLineMax;
  inLineMax = 0;
  n++;
  j++;
  } else {
  if (inLineMax < matrix[i]) inLineMax = matrix[i];
  }
  }
  return result;
}

std::vector<int> getLineMaxParallel(const std::vector<int>matrix, int rows, int columns) {
  if (rows <= 0 || columns <= 0) throw "Matrix is incrorrect";

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int part_size = rows / size;
  const int remain = rows % size;
  if (rank == 0) {
  for (int proc = 1; proc < size; proc++) {
  MPI_Send(matrix.data() + remain * columns + proc * part_size * columns,
  part_size * columns, MPI_INT, proc, 0, MPI_COMM_WORLD);
  }
  }

  int local_size = 0;
  if (rank == 0)
  local_size = (part_size + remain) * columns;
  else
  local_size = part_size * columns;
  std::vector<int> local_vec(local_size);
  if (rank == 0) {
  local_vec = std::vector<int>(matrix.begin(), matrix.begin() + local_size);
  } else {
  MPI_Status status;
  MPI_Recv(local_vec.data(), local_size,
  MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }
  int local_rows = part_size;
  if (rank == 0)
  local_rows += remain;
  std::vector<int> local_max(local_rows);
  if (local_rows > 0 && columns > 0)
  local_max = getLineMaxSequental(local_vec, local_rows, columns);

  if (rank != 0) {
  MPI_Send(local_max.data(), part_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

  std::vector<int> res(rows);

  if (rank == 0) {
  for (int i = 0; i < remain + part_size; i++)
  res[i] = local_max[i];
  for (int i = 1; i < size; i++) {
  MPI_Status status;
  MPI_Recv(res.data() + remain + i * part_size, part_size, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
  }
  }
  return res;
}

