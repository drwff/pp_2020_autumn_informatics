// Copyright 2020 Kuznetsov Nikita
#include <mpi.h>
#include <vector>
#include <ctime>
#include <random>
#include "../../../modules/task_2/kuznetsov_gaussian_vertical/gaussian_vertical.h"

std::vector<double> getMatrix(int x) {
  if (x <= 0)
    throw - 1;
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<double> matrix(x * (x + 1));
  for (int i = 0; i < x * (x + 1); i++) {
    matrix[i] = (gen() % 5) + 1;
  }
  return matrix;
}

std::vector<double> sequentialMethod(std::vector<double> mat, int x) {
  if (static_cast<int>(mat.size()) != (x + 1) * x || x <= 0)
    throw - 1;
  for (int i = 0; i < x; i++) {
    double tmp = mat[i * (x + 1) + i];
    for (int j = x; j >= i; j--)
      mat[i * (x + 1) + j] /= tmp;
    for (int j = i + 1; j < x; j++) {
      tmp = mat[j * (x + 1) + i];
      for (int k = x; k >= i; k--)
        mat[j * (x + 1) + k] -= tmp * mat[i * (x + 1) + k];
    }
  }
  std::vector<double> res(x);
  res[x - 1] = mat[(x - 1) * (x + 1) + x];
  for (int i = x - 2; i >= 0; i--) {
    res[i] = mat[i * (x + 1) + x];
    for (int j = i + 1; j < x; j++)
      res[i] -= mat[i * (x + 1) + j] * res[j];
  }
  return res;
}

std::vector<double> parallelMethod(std::vector<double> mat, int x) {
  if (static_cast<int>(mat.size()) != (x + 1) * x || x <= 0)
    throw - 1;
  int size, rank;
  const int y = x + 1;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int del = y / size;
  const int remain = y % size;
  std::vector<double> vec;
  if (rank < remain)
    vec.resize((del + 1) * x);
  else
    vec.resize(del * x);
  if (rank == 0) {
    for (int process = size - 1; process >= 0; --process) {
      int index = 0;
      for (int j = process; j < y; j += size) {
        for (int i = 0; i < x; ++i) {
          vec[index++] = mat[i * y + j];
        }
      }
      if (process > 0) {
        MPI_Send(vec.data(), index, MPI_DOUBLE, process, 1, MPI_COMM_WORLD);
      }
    }
  } else {
    MPI_Status status;
    MPI_Recv(vec.data(), vec.size(), MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
  }
  std::vector<double> tmp_y(x);
  for (int row = 0; row < x; ++row) {
    if (static_cast<int>(row) % size == rank) {
      int index = 0;
      for (int i = x * (row / size); i < x * (row / size + 1); ++i) {
        tmp_y[index++] = vec[i];
      }
    }
    MPI_Bcast(tmp_y.data(), x, MPI_DOUBLE, row % size, MPI_COMM_WORLD);
    int count;
    if (rank < remain)
      count = del + 1;
    else
      count = del;
    for (int j = row / size; j < count; ++j) {
      double tmp = vec[j * x + row];
      for (int k = 0; k < x; ++k) {
        if (k == row) {
          vec[j * x + k] /= tmp_y[row];
        } else {
          vec[j * x + k] -= tmp * tmp_y[k] / tmp_y[row];
        }
      }
    }
  }
  if (rank == (y - 1) % size) {
    MPI_Request req;
    MPI_Isend(vec.data() + ((y - 1) / size) * x, x, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &req);
  }
  if (rank == 0) {
    vec.resize(x);
    MPI_Status status;
    MPI_Recv(vec.data(), x, MPI_DOUBLE, (y - 1) % size, 0, MPI_COMM_WORLD, &status);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  return vec;
}
