#include <stdio.h>
#include <mpi.h>

int main (int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int myRank;
    int mySize;

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &mySize);

    printf("My rank is: %d --- with size: %d\n", myRank, mySize);

    MPI_Finalize();

    return 0;
}