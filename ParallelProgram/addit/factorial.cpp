#include <mpi.h>
#include <stdio.h>

// Количество цифр после запятой
#define NEG_ACC 500
// Количество цифр до запятой
#define POS_ACC NEG_ACC
#define MAX_SIZE NEG_ACC+POS_ACC+10
#include "long_arith.h"

void myProd(void *a, void *b, int *len, MPI_Datatype *dtype) {
    struct LongNum* in;
    struct LongNum* inout;
    in = (struct LongNum*) a;
    inout = (struct LongNum*) b;
    struct LongNum in_n, inout_n;
    in_n = *in;
    inout_n = *inout;
    LNaddLN(inout_n, in_n);
    *inout = inout_n;
}

int main(int argc, char *argv[], char *env[]) {
    int	N;
    int	start, end, diff, rest;
    double start_time, end_time;
    struct LongNum sum, result, part_fac, add_fac, fac, dop;
    part_fac = toLN(1); add_fac = toLN(1); fac = toLN(1);

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
        LNmulLN(part_fac, toLN(iter));
        dop = toLN(1);
        LNdivLN(dop, part_fac);
        LNaddLN(sum, dop);
    }

    char part_fac_str[MAX_SIZE];
    char fac_str[MAX_SIZE];
    if (rank == 0) {
        if (size != 1) {
            LNoutputSTR(part_fac, part_fac_str);
            MPI_Send(&part_fac_str, strlen(part_fac_str)+1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&fac_str, MAX_SIZE, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, &stat);
        fac = LNinputSTR(fac_str);
        LNmulLN(part_fac, fac);
        if (rank != size - 1) {
            LNoutputSTR(part_fac, part_fac_str);
            MPI_Send(&part_fac_str, strlen(part_fac_str)+1, MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD);
        }
    }

    LNdivLN(sum, fac);
//    char sum_str[MAX_SIZE];
    char result_str[MAX_SIZE];

//    if (rank == 0) {
//        LNaddLN(result, sum);
//        if (size != 1) {
//            int y = 1;
//            for (y; y < size; y++) {
//                MPI_Recv(&sum_str, MAX_SIZE, MPI_CHAR, y, 0, MPI_COMM_WORLD, &stat);
//                sum = LNinputSTR(sum_str);
//                LNaddLN(result, sum);
//            }
//        }
//    } else {
//        LNoutputSTR(sum, sum_str);
//        MPI_Send(&sum_str, strlen(sum_str)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
//    }
    MPI_Op myOp;
    MPI_Datatype ctype;

    MPI_Type_contiguous(MAX_SIZE, MPI_CHAR, &ctype);
    MPI_Type_commit(&ctype);

    MPI_Op_create(myProd, true, &myOp);

    MPI_Reduce(&sum, &result, 1, ctype, myOp, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();
    if (rank == 0) {
        LNaddLN(result, toLN(1));
        LNoutputSTR(result, result_str);
        cout << "Get: ";
        cout << result_str;
        cout << endl;
        printf("With %d; for %f\n", size, end_time-start_time);
    }
    MPI_Finalize();

    return 0;
}
