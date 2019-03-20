#include <mpi.h>
#include <stdio.h>
//#include "long_arith/LongNumber.h"
#include <gmp.h>

int main(int argc, char *argv[], char *env[]) {
    int	N;
    int	start, end, diff, rest;
    double start_time, end_time;
    double sum, result;
    int part_fac = 1, add_fac = 1, fac = 1;

    if (argc != 2) {
        N = 30;
    } else {
        N = atoi(argv[1]);
    }
    MPI_Init(&argc, &argv);
    MPI_Status stat;
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    start_time = MPI_Wtime();

    diff = N / size;
    rest = N % size;
    if (rest != 0) {
        if (rank < rest) {
            diff++;
            start = rank * diff;
            end = start + diff - 1;
        } else {
            start = rank * diff + rest;
            end = start + diff - 1;
        }
    } else {
        start = rank * diff;
        end = start + diff - 1;
    }
    if (start == 0) {
        start = 1;
    }

    int	iter;
    for (iter = start; iter <= end; iter++)
    {
        part_fac = part_fac * iter;
        sum += 1.0 / part_fac;
    }

    if (rank == 0) {
        fac = part_fac;
        if (size != 1) {
            MPI_Send(&fac, 1, MPI_INTEGER, 1, 0, MPI_COMM_WORLD);
        }
        add_fac = 1;
    } else {
        MPI_Recv(&add_fac, 1, MPI_INTEGER, rank - 1, 0, MPI_COMM_WORLD, &stat);
        fac = part_fac * add_fac;
        if (rank != size - 1) {
            MPI_Send(&fac, 1, MPI_INTEGER, rank + 1, 0, MPI_COMM_WORLD);
        }
    }
    sum = sum / add_fac;
    MPI_Reduce(&sum, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Get: %f; with %d; for %f\n", result + 1.0, size, end_time-start_time);
    }

    MPI_Finalize();

    return 0;
}
