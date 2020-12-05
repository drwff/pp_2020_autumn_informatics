// Copyright 2020 Zoreev Mikhail
#include "../../../modules/task_3/zoreev_m_moore_algorithm/moore_algorithm.h"

void randomGraphWithPath(size_t size, int64_t *graph) {
    std::mt19937 generator;
    generator.seed(static_cast<unsigned int>(time(0)));
    for (size_t i = 0; i < size * size; i++) {
        graph[i] = INT64_MIN;
    }
    for (size_t i = 0; i < size - 1; i++) {
        graph[i * size + i + 1] = generator() % 10 + 1;
        graph[(i + 1) * size + i] = generator() % 10 + 1;
    }
    for (size_t i = 0; i < size * size / 4; i++) {
        graph[(generator() % size) * size + generator() % size] = generator() % 10;
    }
}

void randomCompleteGraph(size_t size, int64_t *graph) {
    std::mt19937 generator;
    generator.seed(static_cast<unsigned int>(time(0)));
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            graph[i * size + j] = generator() % 10 + 1;
        }
    }
    for (size_t i = 0; i < size; i++) {
        graph[i * size + i] = INT64_MIN;
    }
}

void printGraph(size_t size, int64_t *graph) {
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            if (graph[i * size + j] != INT64_MIN) {
                std::cout << std::setw(2) << graph[i * size + j] << ' ';
            } else {
                std::cout << std::setw(2) << 'x' << ' ';
            }
        }
        std::cout << std::endl;
    }
}

void printPredecessor(size_t size, uint64_t *predecessor) {
    for (size_t i = 0; i < size; i++) {
        std::cout << std::setw(2) << predecessor[i] << ' ';
    }
    std::cout << std::endl;
}

void mooreAlgorithm(size_t size, int64_t *graph, int64_t *distance, uint64_t *predecessor, size_t root) {
    if (size < 2) {
        throw std::runtime_error("WRONG SIZE");
    }
    for (size_t i = 0; i < size; i++) {
        distance[i] = INT32_MAX;
        predecessor[i] = SIZE_MAX;
    }
    distance[root] = 0;
    predecessor[root] = root;

    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = 0; j < size; j++) {
            for (size_t k = 0; k < j; k++) {
                if ((graph[j * size + k] != INT64_MIN) && (distance[k] > distance[j] + graph[j * size + k])) {
                    distance[k] = distance[j] + graph[j * size + k];
                    predecessor[k] = j;
                }
            }
            for (size_t k = j + 1; k < size; k++) {
                if ((graph[j * size + k] != INT64_MIN) && (distance[k] > distance[j] + graph[j * size + k])) {
                    distance[k] = distance[j] + graph[j * size + k];
                    predecessor[k] = j;
                }
            }
        }
    }
}

void mooreAlgorithmParallel(size_t size, int64_t *graph, int64_t *distance, uint64_t *predecessor, size_t root) {
    int rank, process_count;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size < 2) {
        throw std::runtime_error("WRONG SIZE");
    }
    int64_t *distance_buffer = new int64_t[size * process_count];
    uint64_t *predecessor_buffer = new uint64_t[size * process_count];
    for (size_t i = 0; i < size; i++) {
        distance[i] = INT32_MAX;
        predecessor[i] = SIZE_MAX;
    }
    distance[root] = 0;
    predecessor[root] = root;

    size_t part = size / static_cast<size_t>(process_count);
    size_t start = rank * part, end = (rank + 1) * part;
    if (rank == process_count - 1) {
        end = size;
    }
    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = start; j < end; j++) {
            for (size_t k = 0; k < j; k++) {
                if ((graph[j * size + k] != INT64_MIN) && (distance[k] > distance[j] + graph[j * size + k])) {
                    distance[k] = distance[j] + graph[j * size + k];
                    predecessor[k] = j;
                }
            }
            for (size_t k = j + 1; k < size; k++) {
                if ((graph[j * size + k] != INT64_MIN) && (distance[k] > distance[j] + graph[j * size + k])) {
                    distance[k] = distance[j] + graph[j * size + k];
                    predecessor[k] = j;
                }
            }
        }

        MPI_Allgather(distance, static_cast<int>(size), MPI_INT64_T, distance_buffer, static_cast<int>(size),
                      MPI_INT64_T, MPI_COMM_WORLD);
        MPI_Allgather(predecessor, static_cast<int>(size), MPI_UINT64_T, predecessor_buffer, static_cast<int>(size),
                      MPI_UINT64_T, MPI_COMM_WORLD);
        for (size_t k = 0; k < size; k++) {
            for (size_t j = 0; j < static_cast<size_t>(process_count); j++) {
                if (distance_buffer[j * size + k] < distance[k]) {
                    distance[k] = distance_buffer[j * size + k];
                    predecessor[k] = predecessor_buffer[j * size + k];
                }
            }
        }
    }
    delete[] distance_buffer;
    delete[] predecessor_buffer;
}
