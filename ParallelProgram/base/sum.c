#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[], char *env[]) {
    int	N = 1000000000;
    int	start, end;
    double start_time, end_time;
    double sum, result;

    if (argc != 2) {
        N = 1000;
    } else {
        N = atoi(argv[1]);
    }

    MPI_Init(&argc, &argv);


    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    start_time = MPI_Wtime();

    int diff = N / size;
    if (rank == 0) {
        start = 1;
        end = diff;
    } else if (rank == size - 1) {
        start = rank * diff;
        end = N;
    } else {
        start = rank * diff;
        end = (rank + 1) * diff;
    }

    int	i;
    for (i = start; i <= end; i++)
    {
        sum += 1.0 / i;
    }
    MPI_Reduce(&sum, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    end_time = MPI_Wtime();
    if (rank == 0)
    {
        printf("Get: %f; with %d; for %f\n", result, size, end_time-start_time);
    }

    MPI_Finalize();

    return 0;
}
