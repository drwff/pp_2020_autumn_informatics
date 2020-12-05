// Copyright 2020 Tarasov Nikita
#include <mpi.h>
#include <ctime>
#include <iostream>
#include <vector>
#include <random>
#include "../../../modules/task_2/tarasov_n_change_contrast/change_contrast.h"

#define CLR 256

std::vector<int> set_random_img(int _width, int _high) {
    int _size_img = _width * _high;
    if (_size_img < 1) throw "Error";
    std::default_random_engine random;
    random.seed(static_cast<unsigned int>(std::time(0)));
    std::vector<int> pic(_size_img);
    for (int i = 0; i < _size_img; i++) {
        pic[i] = random() % CLR;
    }
    return pic;
}

void print_img(std::vector<int> pic, int _high, int _width) {
    for (int i = 0; i < _high; i++) {
        for (int j = 0; j < _width; j++)
            std::cout << pic[i * _high + j] << " ";
        std::cout << std::endl;
    }
}

std::vector<int> changeContrast(const std::vector<int> &pic, int _width, int _high, int _correction) {
    int _size_img = _width * _high;
    if (_width * _high != static_cast<int>(pic.size())) throw "Error";
    if (_size_img < 1) throw "Error";
    std::vector<int> color_pallete(CLR);

    if (_correction == 0) return pic;

    int lAB = 0;

    for (int i = 0; i < _size_img; i++)
        lAB += pic[i];

    lAB = lAB / _size_img;

    double k = 1.0 + _correction / 100.0;

    for (int i = 0; i < CLR; i++) {
        int delta = static_cast<int>(i) - lAB;
        int temp = static_cast<int>(lAB + k * delta);

        if (temp < 0)
            temp = 0;

        if (temp >= 255)
            temp = 255;
        color_pallete[i] = static_cast<char>(temp);
    }

    std::vector<int> result(_size_img);
    for (int i = 0; i < _size_img; i++) {
        unsigned char value = pic[i];
        result[i] =  static_cast<unsigned char>(color_pallete[value]);
    }

    return result;
}

std::vector<int> changeContrastParallel(const std::vector<int> &pic, int _width, int _high, int _correction) {
    int _size_img = _width * _high;
    if (_width * _high != static_cast<int>(pic.size())) throw "Error";
    if (_size_img < 1) throw "Error";
    if (_correction == 0) return pic;

    int mpisize, mpirank;

    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpisize);

    std::vector<int> result(_size_img);

    int dist = _size_img / mpisize;
    int rem = _size_img % mpisize;
    int mpiroot = 0;

    if (dist < 1) {
        if (mpirank == mpiroot) result = changeContrast(pic, _width, _high , _correction);
        return result;
    }

    int pic_scount_size = mpisize;
    int pic_displs_size = mpisize;

    std::vector<int> pic_sbuf = pic;
    std::vector<int> pic_scount(pic_scount_size, 0);
    std::vector<int> pic_displs(pic_displs_size, 0);

    std::vector<int>color_pallete(CLR);
    int lAB = 0;

    if (rem != 0)
        pic_scount[0] = dist + rem;
    else
        pic_scount[0] = dist;
    for (int i = 1; i < pic_scount_size; i++)
        pic_scount[i] = dist;

    for (int i = 1; i < pic_displs_size; i++)
        pic_displs[i] = pic_displs[i - 1] + pic_scount[i - 1];


    std::vector<int> rec_pic(pic_scount[mpirank], 0);
    MPI_Scatterv(&pic_sbuf[0], &pic_scount[0], &pic_displs[0], MPI_INT, &rec_pic[0],
        pic_scount[mpirank], MPI_INT, mpiroot, MPI_COMM_WORLD);

    int sum;
    int rec_lAB = 0;

    int rec_pic_size = static_cast<int>(rec_pic.size());

    for (int i = 0; i < rec_pic_size; i++)
        rec_lAB += rec_pic[i];

    MPI_Reduce(&rec_lAB, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (mpirank == mpiroot) {
        lAB = sum / _size_img;
        double k = 1.0 + _correction / 100.0;

        for (int i = 0; i < CLR; i++) {
            int delta = static_cast<int>(i) - lAB;
            int temp = static_cast<int>(lAB + k * delta);

            if (temp < 0)
                temp = 0;

            if (temp >= 255)
                temp = 255;
            color_pallete[i] = static_cast<char>(temp);
        }
    }

    MPI_Bcast(&color_pallete[0], CLR, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < rec_pic_size; i++) {
        unsigned char value = rec_pic[i];
        rec_pic[i] = static_cast<unsigned char>(color_pallete[value]);
    }

    MPI_Gatherv(&rec_pic[0], pic_scount[mpirank], MPI_INT, &result[0], &pic_scount[0], &pic_displs[0],
        MPI_INT, mpiroot, MPI_COMM_WORLD);

    return result;
}
