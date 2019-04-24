#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[]) {
    double start_time, end_time;
    int start, end, diff, rest;
    int size, rank;
    int i, j, p;

    srand(static_cast<unsigned int>(time(0)));

    int length, max_time;
    if (argc < 3) {
        length = 10;
        max_time = 2;
    } else {
        length = atoi(argv[1]);
        max_time = atoi(argv[2]);
    }
    short int result1[length][length];
//    length = 5;
//    short int result1[5][5] = {
//            {0, 0, 0, 0, 0},
//            {0, 0, 1, 0, 0},
//            {0, 0, 1, 0, 0},
//            {0, 0, 1, 0, 0},
//            {0, 0, 0, 0, 0}
//    }; // повтораяющаяся картинка
//    length = 10;
//    short int result1[10][10] = {
//            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//            {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
//            {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//            {0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
//            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
//    }; // 41 итерация нужна и +4 чтобы получить прежнее состояние
    short int result2[length][length];
    for (i = 0; i < length; i++) {
        for (j = 0; j < length; j++) {
            result1[i][j] = rand() % 2;
            result2[i][j] = result1[i][j];
        }
    }

    int count_near;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Request req1, req2;
    MPI_Status status;
    start_time = MPI_Wtime();

    diff = length / size;
    rest = length % size;
    if (rank < rest) {
        diff++;
        start = rank * diff;
        end = start + diff - 1;
    } else {
        start = rank * diff + rest;
        end = start + diff - 1;
    }

    int prev = rank - 1, next = rank + 1, prev_iter = start - 1, next_iter = end + 1;
    if (rank == 0) {
        prev = size - 1;
        prev_iter = length - 1;
    } else if (rank == size - 1) {
        next = 0;
        next_iter = 0;
    }

    if (rank == 0) {
        for (j = 0; j < length; j++) {
            for (i = 0; i < length; i++) {
                printf("%d ", result1[j][i]);
            }
            printf("\n");
        }
        printf("\n\n");
    }

    for (i = 1; i < max_time; i++) {
        for (j = start; j <= end; j++) {
            for (p = 0; p < length; p++) {
                if (j == 0) {
                    count_near = result1[length - 1][p] + result1[j + 1][p];
                    if (p == 0) {
                        count_near += result1[j][length - 1] + result1[j][p + 1];
                        count_near += result1[length - 1][length - 1] + result1[length - 1][p + 1] +
                                      result1[j + 1][length - 1] + result1[j + 1][p + 1];
                    } else if (p == length - 1) {
                        count_near += result1[j][p - 1] + result1[j][0];
                        count_near += result1[length - 1][p - 1] + result1[length - 1][0] +
                                      result1[j + 1][p - 1] + result1[j + 1][0];
                    } else {
                        count_near += result1[j][p - 1] + result1[j][p + 1];
                        count_near += result1[length - 1][p - 1] + result1[length - 1][p + 1] +
                                      result1[j + 1][p - 1] + result1[j + 1][p + 1];
                    }
                } else if (j == length - 1) {
                    count_near = result1[j - 1][p] + result1[0][p];
                    if (p == 0) {
                        count_near += result1[j][length - 1] + result1[j][p + 1];
                        count_near += result1[j - 1][length - 1] + result1[j - 1][p + 1] +
                                      result1[0][length - 1] + result1[0][p + 1];
                    } else if (p == length - 1) {
                        count_near += result1[j][p - 1] + result1[j][0];
                        count_near += result1[j - 1][p - 1] + result1[j - 1][0] +
                                      result1[0][p - 1] + result1[0][0];
                    } else {
                        count_near += result1[j][p - 1] + result1[j][p + 1];
                        count_near += result1[j - 1][p - 1] + result1[j - 1][p + 1] +
                                      result1[0][p - 1] + result1[0][p + 1];
                    }
                } else {
                    count_near = result1[j - 1][p] + result1[j + 1][p];
                    if (p == 0) {
                        count_near += result1[j][length - 1] + result1[j][p + 1];
                        count_near += result1[j - 1][length - 1] + result1[j - 1][p + 1] +
                                      result1[j + 1][length - 1] + result1[j + 1][p + 1];
                    } else if (p == length - 1) {
                        count_near += result1[j][p - 1] + result1[j][0];
                        count_near += result1[j - 1][p - 1] + result1[j - 1][0] +
                                      result1[j + 1][p - 1] + result1[j + 1][0];
                    } else {
                        count_near += result1[j][p - 1] + result1[j][p + 1];
                        count_near += result1[j - 1][p - 1] + result1[j - 1][p + 1] +
                                result1[j + 1][p - 1] + result1[j + 1][p + 1];
                    }
                }

                if ((count_near == 2 || count_near == 3) && result1[j][p] == 1) {
                    result2[j][p] = 1;
                } else if (count_near == 3 && result1[j][p] == 0){
                    result2[j][p] = 1;
                } else {
                    result2[j][p] = 0;
                }
            }
        }

        if (size != 1) {
            MPI_Sendrecv(&result2[start], length, MPI_INTEGER, prev, 0,
                         &result1[next_iter], length, MPI_INTEGER, next, 0,
                         MPI_COMM_WORLD, &status);
            MPI_Sendrecv(&result2[end], length, MPI_INTEGER, next, 0,
                         &result1[prev_iter], length, MPI_INTEGER, prev, 0,
                         MPI_COMM_WORLD, &status);
        }

        for (j = start; j <= end; j++) {
            for (p = 0; p < length; p++) {
                result1[j][p] = result2[j][p];
            }
        }
    }

    end_time = MPI_Wtime();

//    for (j = 0; j < length; j++) {
//        for (i = 0; i < length; i++) {
//            printf("%d ", result2[j][i]);
//        }
//        printf("\n");
//    }
//    printf("%d -- \n\n", rank);

    int rcounts[size], displs[size];
    int prev_dis = 0;
    diff = length / size;
    for (i = 0; i < size; i++) {
        if (i < rest) {
            rcounts[i] = (diff + 1) * length;
        } else {
            rcounts[i] = diff * length;
        }
        displs[i] = prev_dis;
        prev_dis += rcounts[i];
    }

    if (rank == 0) {
        for (i = 1; i < size; i++) {
            for (j = 0; j < rcounts[i]; j++) {
                MPI_Recv(&result2[(displs[i] + j) / length][j % length], 1, MPI_INTEGER, i, 0, MPI_COMM_WORLD, &status);
            }
        }

        for (j = 0; j < length; j++) {
            for (i = 0; i < length; i++) {
                printf("%d ", result2[j][i]);
            }
            printf("\n");
        }
        printf("In time %d; with %d; for %f\n", max_time, size, end_time - start_time);
    } else {
        for (j = start; j <= end; j++) {
            for (i = 0; i < length; i++) {
                MPI_Send(&result2[j][i], 1, MPI_INTEGER, 0, 0, MPI_COMM_WORLD);
            }
        }
    }
    MPI_Finalize();

    return 0;
}