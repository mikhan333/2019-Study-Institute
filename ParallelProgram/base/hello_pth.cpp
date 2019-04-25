#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int size;

typedef struct  {
    int rank;
} thread_args;

void *thread_func(void *arg) {
    pthread_t this_thread;
    this_thread = pthread_self();

    auto *point = (thread_args*) arg;

    printf("Thread = %lu; size = %d; rank = %d\n", this_thread, size, point->rank);

    return nullptr;
}


int main(int argc, char *argv[]) {
    int err;
    if (argc != 2) {
        size = 4;
    } else {
        size = atoi(argv[1]);
    }

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

    pthread_t this_thread;
    this_thread = pthread_self();
    printf("Thread = %lu; size = %d; rank = main\n", this_thread, size);

    for (int i = 0; i < size; i++) {
        pthread_join(threads[i], (void **) nullptr);
    }

    return 0;
}