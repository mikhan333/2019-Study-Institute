#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[], char *env[]) {
    double start_time, end_time;
    int var = 2;

    MPI_Init(&argc, &argv);
    MPI_Status stat;
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    start_time = MPI_Wtime();

    if (rank == 0) {
        MPI_Send(&var, 1, MPI_INTEGER, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&var, 1, MPI_INTEGER, size - 1, 0, MPI_COMM_WORLD, &stat);
        printf("I get: %d; rank %d\n", var, rank);
    }
    else {
        MPI_Recv(&var, 1, MPI_INTEGER, rank - 1, 0, MPI_COMM_WORLD, &stat);
        printf("I get: %d; rank %d\n", var, rank);
        var = var * 2;
        if (rank == size - 1) {
            MPI_Send(&var, 1, MPI_INTEGER, 0, 0, MPI_COMM_WORLD);
        } else {
            MPI_Send(&var, 1, MPI_INTEGER, rank + 1, 0, MPI_COMM_WORLD);
        }
    }
    end_time = MPI_Wtime();
    if (rank == 0) {
        printf("All-Get: %d; with %d; for %f\n", var, size, end_time-start_time);
    }

    MPI_Finalize();

    return 0;
}
