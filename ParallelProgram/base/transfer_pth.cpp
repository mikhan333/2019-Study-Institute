#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

pthread_mutex_t mutex;

int size;

static int shared = 0;

typedef struct {
    int rank;
} thread_args;

void *thread_func(void *arg) {
    auto *point = (thread_args *) arg;
    int rank = point->rank;
    int local = 0;

    while (shared != rank);
    pthread_mutex_lock(&mutex);
    local = shared++;
    pthread_mutex_unlock(&mutex);

    printf("Get = %d; rank = %d\n", local, rank);

    return nullptr;
}

int main(int argc, char *argv[]) {
    pthread_mutex_init(&mutex, NULL);

    int err;
    if (argc != 3) {
        size = 10;
    } else {
        size = atoi(argv[1]);
    }

    unsigned int start_time = clock();

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
    printf("Result = %d; time = %f; rank = main\n", shared, search_time);

    return 0;
}
