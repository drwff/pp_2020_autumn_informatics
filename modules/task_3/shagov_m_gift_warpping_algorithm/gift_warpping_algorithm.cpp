// Copyright 2020 Shagov Maksim
#include <vector>
#include "../../../modules/task_3/shagov_m_gift_warpping_algorithm/gift_warpping_algorithm.h"

std::vector<Point> createRandomPoints(size_t size) {
    std::vector<Point> result(size);
    std::random_device rd;
    std::mt19937 mersenne(rd());
    std::uniform_real_distribution<> urd(-100000.0, 100000.0);
    for (size_t i = 0; i < size; i++) {
        result[i].x = static_cast<int>(urd(mersenne));
        result[i].y = static_cast<int>(urd(mersenne));
    }
    return result;
}

bool pointCheck(const Point& back, const Point& current, const Point& challenger) {
    double x;

    if ((challenger.x - current.x) == 0) {
        if (challenger.x == back.x) {
            if ((challenger.y - current.y) > 0) {
                if (challenger.y > back.y) {
                    return true;
                } else {
                    return false;
                }
            }
            if ((challenger.y - current.y) < 0) {
                if (challenger.y < back.y) {
                    return true;
                } else {
                    return false;
                }
            }
        }
        x = challenger.x;
    } else {
        if ((challenger.y - current.y) == 0) {
            x = back.x;
            if (challenger.x > current.x) {
                if (challenger.y == back.y) {
                    if (back.x > challenger.x) {
                        return false;
                    } else {
                        if ((back.x == challenger.x) && (challenger.y > back.y)) {
                            return false;
                        }
                        return true;
                    }
                } else {
                    return challenger.y <= back.y;
                }
            } else {
                if (challenger.x < current.x) {
                    if (challenger.y == back.y) {
                        if (back.x > challenger.x) {
                            return true;
                        } else {
                            if ((back.x == challenger.x) && (back.y > challenger.y)) {
                                return true;
                            }
                            return false;
                        }
                    } else {
                        return !(challenger.y <= back.y);
                    }
                }
            }
        } else {
            double tan1 = (static_cast<double>(challenger.y - current.y)) /
            (static_cast<double>(challenger.x - current.x));
            double tan2 = tan1 + 1.0;
            if ((back.x - current.x) != 0) {
                tan2 = (back.y - current.y) / (back.x - current.x);
            }
            if (tan1 == tan2) {
                if (challenger.y > current.y) {
                    return challenger.y > back.y;
                } else {
                    return challenger.y < back.y;
                }
            } else {
                x = static_cast<double>(back.y - challenger.y) /
                tan1 + static_cast<double>(challenger.x);
            }
        }
    }
    if (challenger.y > current.y) {
        if (challenger.x > current.x) {
            return !(x <= back.x);
        } else {
            return x > back.x;
        }
    } else {
        if ((challenger.y - current.y) != 0) {
            return x <= back.x;
        }
    }
    return false;
}

size_t searchMinPoint(const std::vector<Point>& mas) {
    size_t base = 0;
    for (size_t i = 0; i < mas.size(); i++) {
        if (mas[base].x > mas[i].x) {
            base = i;
        } else {
            if ((mas[base].x == mas[i].x) && (mas[base].y > mas[i].y)) {
                base = i;
            }
        }
    }
    return base;
}

Point searchMinPointParallel(const std::vector<Point>& mas, int rank, int procCount) {
    Point local_left_point = mas[searchMinPoint(mas)];
    std::vector<Point> local_points(procCount);
    MPI_Gather(&local_left_point, 2, MPI_INT, &local_points[0], 2, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        local_left_point = local_points[searchMinPoint(local_points)];
    }
    MPI_Bcast(&local_left_point, 2, MPI_INT, 0, MPI_COMM_WORLD);
    return local_left_point;
}

std::vector<Point> buildConvexHullParallel(const std::vector<Point>& mas) {
    double startT = 0;
    int rank, procCount;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    if (rank == 0) {
        startT = MPI_Wtime();
    }
    int elements_count = static_cast<int>(mas.size());
    if ((elements_count == 1) || (elements_count == 2)) {
        return mas;
    }
    int elements_part_count = elements_count / procCount;
    int remainder = elements_count % procCount;
    if ((elements_part_count == 1) || (static_cast<size_t>(procCount) > mas.size())) {
        if (rank == 0) {
            return buildConvexHull(mas);
        } else {
            return mas;
        }
    }
    int* revcount = new int[procCount];
    int* displs = new int[procCount];
    revcount[0] = 2 * (elements_part_count + remainder);
    displs[0] = 0;
    for (int i = 1; i < procCount; i++) {
        revcount[i] = elements_part_count * 2;
        displs[i] = (elements_part_count * i + remainder) * 2;
    }
    std::vector<Point> part_vector(rank == 0 ? elements_part_count + remainder : elements_part_count);
    MPI_Scatterv(&mas[0], revcount, displs, MPI_INT, &part_vector[0], revcount[rank], MPI_INT, 0, MPI_COMM_WORLD);
    Point left_point = searchMinPointParallel(part_vector, rank, procCount);
    std::vector<Point> convex_hull;
    convex_hull.push_back(left_point);
    std::vector<Point> local_points(procCount);
    Point next;
    do {
        next = part_vector[0];
        if (next == convex_hull.back()) {
            next = part_vector[1];
        }
        for (int i = 0; i < static_cast<int>(part_vector.size()); i++) {
            if ((part_vector[i] == convex_hull.back()) || (convex_hull.back() == next)) {
                continue;
            }
            if (pointCheck(convex_hull.back(), next, part_vector[i]) || (part_vector[i] == next)) {
                next = part_vector[i];
            }
        }
        MPI_Gather(&next, 2, MPI_INT, &local_points[0], 2, MPI_INT, 0, MPI_COMM_WORLD);
        if (rank == 0) {
            for (int i = 0; i < static_cast<int>(local_points.size()); i++) {
                if (pointCheck(convex_hull.back(), next, local_points[i])) {
                    next = local_points[i];
                }
            }
        }
        MPI_Bcast(&next, 2, MPI_INT, 0, MPI_COMM_WORLD);
        convex_hull.push_back(next);
        if (convex_hull.size() == mas.size()) {
            break;
        }
    } while (convex_hull.back() != left_point);
    if (rank == 0) {
        double endT = MPI_Wtime();
        std::cout << "MPI time: " << endT - startT << std::endl;
    }
    return convex_hull;
}

std::vector<Point> buildConvexHull(const std::vector<Point>& mas) {
    if ((mas.size() == 1) || (mas.size() == 2)) {
        return mas;
    }
    std::vector<Point> part_vector(mas);
    Point left_point = part_vector[searchMinPoint(part_vector)];
    std::vector<Point> convex_hull;
    convex_hull.push_back(left_point);
    Point next;
    do {
        next = part_vector[0];
        if (next == convex_hull.back()) {
            next = part_vector[1];
        }
        for (int i = 0; i < static_cast<int>(part_vector.size()); i++) {
            if ((part_vector[i] == convex_hull.back()) || (convex_hull.back() == next)) {
                continue;
            }
            if ((part_vector[i] == next) || pointCheck(convex_hull.back(), next, part_vector[i])) {
                next = part_vector[i];
            }
        }
        convex_hull.push_back(next);
        if (convex_hull.size() == mas.size()) {
            break;
        }
    } while (convex_hull.back() != left_point);

    return convex_hull;
}
