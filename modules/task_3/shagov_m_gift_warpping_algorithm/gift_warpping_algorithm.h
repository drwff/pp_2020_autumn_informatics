// Copyright 2020 Shagov Maksim
#ifndef MODULES_TASK_3_SHAGOV_M_GIFT_WARPPING_ALGORITHM_GIFT_WARPPING_ALGORITHM_H_
#define MODULES_TASK_3_SHAGOV_M_GIFT_WARPPING_ALGORITHM_GIFT_WARPPING_ALGORITHM_H_
#include <mpi.h>
#include <vector>
#include <iostream>
#include <ctime>
#include <random>

struct Point {
    int x, y;
    Point() = default;
    Point(int X, int Y) {
        x = X;
        y = Y;
    }

    const Point operator=(const Point& point) {
        x = point.x;
        y = point.y;
        return *this;
    }

    bool operator!=(const Point& point) {
        return ((x != point.x) || (y != point.y));
    }

    bool operator==(const Point& point) {
        return ((x == point.x) && (y == point.y));
    }
};

std::vector<Point> createRandomPoints(size_t size);
size_t searchMinPoint(const std::vector<Point> &mas);
Point searchMinPointParallel(const std::vector<Point> &mas, int rank, int procCount);
std::vector<Point> buildConvexHullParallel(const std::vector<Point> &mas);
std::vector<Point> buildConvexHull(const std::vector<Point> &mas);
bool pointCheck(const Point& back, const Point& current, const Point& challenger);
#endif  // MODULES_TASK_3_SHAGOV_M_GIFT_WARPPING_ALGORITHM_GIFT_WARPPING_ALGORITHM_H_
