#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <unistd.h>

pthread_mutex_t mutex;
double interval_start = 0.001, interval_end = 1;

int size;
int diff, rest;

int points;

static double result = 0;

// we can define count of points in which function sin(1/x) is equal to 0
int count_zero_points = (int) ((1 / M_PI) / interval_start);
int count_zero_per_one;
// we give same number of "zero's points" to threads
// in our situation count_zero_points === 318
auto get_coord = [](int rank) { return ((1 / M_PI) / (count_zero_per_one * (size - rank - 1))); };


typedef struct {
    int rank;
} thread_args;


double func_integral(double coord_left, double inter) {
    return inter * (sin(1 / coord_left) + sin(1 / (coord_left + inter))) / 2;
}


void *thread_func(void *arg) {
    auto *point = (thread_args *) arg;
    int rank = point->rank;

    double sum = 0;

    int start, end;
    if (rank < rest) {
        start = rank * (diff + 1);
        end = start + diff;
    } else {
        start = rank * diff + rest;
        end = start + diff - 1;
    }

    double inter;
    double coord_start, coord_end;
    if (rank == 0) {
        coord_start = interval_start;
        if (size == 1) {
            coord_end = interval_end;
        } else {
            coord_end = get_coord(rank);
        }
    } else if (rank == size - 1) {
        coord_start = get_coord(rank - 1);
        coord_end = interval_end;
    } else {
        coord_start = get_coord(rank - 1);
        coord_end = get_coord(rank);
    }
    inter = (coord_end - coord_start) / (end - start + 1);

    double coord = coord_start;
    for (int i = start; i <= end; i++) {
        sum += func_integral(coord, inter);
        coord += inter;
    }
    printf("Sum = %.15f; rank = %d; start = %.15f / end = %.15f; inter = %.15f\n", sum, rank, coord_start, coord_end, inter);

    pthread_mutex_lock(&mutex);
    result += sum;
    pthread_mutex_unlock(&mutex);

    return nullptr;
}


int main(int argc, char *argv[]) {
    pthread_mutex_init(&mutex, nullptr);

    int err;
    if (argc != 3) {
        size = 20;
        points = 100000;
    } else {
        size = atoi(argv[1]);
        points = atoi(argv[2]);
    }

    unsigned int start_time = clock();

    diff = points / size;
    rest = points % size;
    count_zero_per_one = count_zero_points / size;

    pthread_t threads[size];
    thread_args threads_args[size];
    for (int i = 0; i < size; i++) {
        threads_args[i].rank = i;
        err = pthread_create(
                &threads[i],
                (pthread_attr_t *) nullptr,
                thread_func,
                &threads_args[i]
        );
        if (err != 0) {
            printf("Error on thread create, return value = %d\n", err);
            exit(-1);
        }
    }

    for (int i = 0; i < size; i++) {
        pthread_join(threads[i], (void **) nullptr);
    }
    pthread_mutex_destroy(&mutex);

    unsigned int end_time = clock();
    double search_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    printf("Result = %.15f; time = %f; rank = main\n", result, search_time);

    return 0;
}
