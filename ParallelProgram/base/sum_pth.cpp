#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

pthread_mutex_t mutex;

int size;
int diff;

int N;

static double result = 0;

typedef struct {
    int rank;
    double part_sum;
} thread_args;

void *thread_func(void *arg) {
    auto *point = (thread_args *) arg;
    int rank = point->rank;

    double sum = 0;
    int start, end;

    if (rank == 0) {
        start = 1;
        end = diff;
    } else if (rank == size - 1) {
        start = rank * diff + 1;
        end = N;
    } else {
        start = rank * diff + 1;
        end = (rank + 1) * diff;
    }

    for (int i = start; i <= end; i++) {
        sum += 1.0 / i;
    }
    point->part_sum = sum;
    printf("Sum = %f; rank = %d; start = %d / end = %d\n", sum, rank, start, end);

    pthread_mutex_lock(&mutex);
    result += sum;
    pthread_mutex_unlock(&mutex);

    return nullptr;
}

int main(int argc, char *argv[]) {
    pthread_mutex_init(&mutex, NULL);

    int err;
    if (argc != 3) {
        size = 3;
        N = 1000;
    } else {
        size = atoi(argv[1]);
        N = atoi(argv[2]);
    }

    unsigned int start_time = clock();

    diff = N / size;
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
//    double result = 0;
//    for (int i = 0; i < size; i++) {
//        result = result + threads_args[i].part_sum;
//    }

    unsigned int end_time = clock();
    double search_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    printf("Result = %.15f; time = %f; rank = main\n", result, search_time);

    return 0;
}
