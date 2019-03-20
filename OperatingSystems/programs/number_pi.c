#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int diff;
int part_one;
int N;
double h;

double funct(double x) {
    return sqrt(4 - x * x);
}

struct Args {
    int param;
    double part_sum;
};

void *mythread(void *arg) {
    pthread_t mythid;
    mythid = pthread_self();
    double sum = 0;
    struct Args *point = arg;
    int rank = point->param;

    int start, end;

    if (rank != N - 1) {
        start = rank * part_one;
        end = (rank + 1) * part_one;
    } else {
        start = rank * part_one;
        end = diff;
    }

    double left, right;

    int i;
    for (i = start; i < end; i++) {
        left = i * h;
        right = (i + 1) * h;
        sum = sum + (funct(left) + funct(right)) * h / 2;
    }

    printf("Thread = %lu, Calculation result = %f, Rank = %d\n", mythid, sum, rank);
    point->part_sum = sum;
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t mythid;
    int err;
    int i;
    double result = 0;

    N = atoi(argv[1]);
    h = atof(argv[2]);

    diff = 2 / h;
    part_one = diff / N;
    struct Args mas[N];
    pthread_t thid[N];
    for (i = 0; i < N; i++) {
        mas[i].param = i;
        err = pthread_create(&thid[i], (pthread_attr_t *) NULL, mythread, &mas[i]);
        if (err != 0) {
            printf("Error on thread create, return value = %d\n", err);
            exit(-1);
        }
    }

    mythid = pthread_self();

    printf("Thread created, main = %lu\n", mythid);

    for (i = 0; i < N; i++) {
        pthread_join(thid[i], (void **) NULL);
    }
    for (i = 0; i < N; i++) {
        result = result + mas[i].part_sum;
    }
    printf("Calculation result = %.15f\n", result);

    return 0;
}
