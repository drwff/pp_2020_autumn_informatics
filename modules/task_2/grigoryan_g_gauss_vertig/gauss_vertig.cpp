// Copyright 2020 Grigoryan Garry
#include <mpi.h>
#include <utility>
#include <vector>
#include <random>
#include <cmath>
#include "../../../modules/task_2/grigoryan_g_gauss_vertig/gauss_vertig.h"

std::vector <double> RandomMatrix(int _size, time_t seed) {
  std::mt19937 gen(seed);
  if (_size <= 0)
    throw "0 or negative size!";
  std::vector <double> matrix(_size*(_size + 1));
  for (int i = 0; i < static_cast<int>(matrix.size()); i++) {
    matrix[i] = gen() % 50;
  }
  return matrix;
}

std::vector<double> OGauss(std::vector<double> matrix, int _size) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> vector(_size);
  std::vector<double> b(_size, 1);
  int segm, ost;
  int ch = Check(matrix, _size);
  if (ch != 0)
    throw "Not double vector";
  if (ch == 0) {
    segm = _size / size;
    ost = _size % size;
    double koef;

    for (int i = _size - 1; i > 0; i--) {
      int fk = 0;
      if ((rank != 0) && (i < segm*size) && (rank == i % size))
        MPI_Send(matrix.data() + _size * _size + i, 1, MPI_DOUBLE, 0, i, MPI_COMM_WORLD);

      if ((i%size != 0) && (i < segm*size) && (rank == 0)) {
        MPI_Status status;
        MPI_Recv(b.data() + i, 1, MPI_DOUBLE, i%size, i, MPI_COMM_WORLD, &status);
        fk = 1;
      }
      if ((fk == 0) && (rank == 0))
        b[i] = matrix[_size*_size + i];

      if (rank == 0) {
        vector[i] = b[i] / matrix[_size*i + i];
        koef = vector[i];
      }
      MPI_Bcast(&koef, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      for (int s = 0; s < segm; s++)
        matrix[_size*_size + s * size + rank] -= koef * matrix[i*_size + s * size + rank];
      if (rank == 0) {
        for (int s = 0; s < ost; s++)
          matrix[_size*_size + segm * size + s] -= koef * matrix[i*_size + segm * size + s];
      }
    }

    if (rank == 0)
      vector[0] = matrix[_size * _size + 0] / matrix[_size * 0 + 0];
  }
  return vector;
}

std::vector<double> ParrG(std::vector<double> matrix, int _size) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int segm = (_size + 1) / size;
  int ost = (_size + 1) % size;

  std::vector <double> maskoef(_size);
  std::vector <double> tmpmaskoef(_size, 1);
  std::vector <double> prom_res(_size*(_size + 1));
  prom_res = matrix;

  maskoef[0] = 1;
  int maxin;

  int f = 1;
  for (int it = 0; it < _size - 1; it++) {
    maxin = maxind(it, _size, matrix);
    for (int i = 0; i < _size + 1; i++) {
      std::swap(matrix[i*_size + it], matrix[i*_size + maxin]);
      std::swap(prom_res[i*_size + it], prom_res[i*_size + maxin]);
    }
    if (rank == 0) {
      for (int i = it + 1; i < _size; i++)
        maskoef[i] = matrix[it*_size + i] / matrix[it*_size + it];
      for (int i = 1; i < size; i++)
        MPI_Send(maskoef.data() + f, _size - (it + 1), MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
      f++;
    } else {
      MPI_Status status;
      MPI_Recv(tmpmaskoef.data() + 1, _size - (it + 1), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }
    if (rank == 0) {
      for (int i = 0; i < segm; i++) {
        for (int j = it + 1; j < _size; j++)
          matrix[(i*size + rank)*_size + j] -= maskoef[j] * matrix[(i*size + rank)*_size + it];
        for (int k = 1; k < size; k++) {
          MPI_Status status;
          MPI_Recv(matrix.data() + (i*size + k)*_size + it + 1, _size - it - 1, MPI_DOUBLE,
        k, 9, MPI_COMM_WORLD, &status);
        }
      }
      for (int i = 0; i < ost; i++) {
        for (int j = it + 1; j < _size; j++)
          matrix[(segm*size + i)*_size + j] -= maskoef[j] * matrix[(segm*size + i)*_size + it];
      }
    } else {
      for (int i = 0; i < segm; i++) {
        f = 1;
        for (int j = it + 1; j < _size; j++) {
          int tmp = i * size + rank;
          prom_res[(tmp)*_size + j] = prom_res[(tmp)*_size + j] - tmpmaskoef[f] * prom_res[(tmp)*_size + it];
          f++;
        }
        MPI_Send(prom_res.data() + (i*size + rank)*_size + it + 1, _size - it - 1, MPI_DOUBLE, 0, 9, MPI_COMM_WORLD);
      }
    }
  }
  return matrix;
}

bool NullStr(std::vector<double> matrix, int str, int _size) {
  for (int i = str; i < _size; i++)
    if (matrix[i*_size + str] != 0)
      return 0;
  return 1;
}

int maxind(int it, int _size, std::vector <double> matrix) {
  double max = std::abs(matrix[it*_size + it]);
  int in = it;
  for (int i = it; i < _size; i++) {
    if (std::abs(matrix[it*_size + i]) > max) {
      max = std::abs(matrix[it*_size + i]);
      in = i;
    }
  }
  return in;
}

int Check(std::vector<double> matrix, int _size) {
  for (int i = 0; i < _size; i++) {
    if (NullStr(matrix, i, _size) == 1) {
      if (matrix[(_size + 1)*_size + i] == 0)
        return i;
      if (matrix[(_size + 1)*_size + i] != 0)
        throw "There are no solutions";
    }
  }
  return 0;
}
