#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
//#include <cmath>

double inter_coord, inter_time;

double function_f(int k, int m) {
    double coord = m * inter_coord;
    double time = k * inter_time;
    return 0;
}

double function_coord(int m) {
    double coord = m * inter_coord;
    return coord;
}

double function_time(int k) {
    double time = k * inter_time;
    return time;
}

double transport_eq(int k, int m, double u_left, double u_mid, double u_right) {
    return (function_f(k - 1, m) + 0.5 * inter_time * (u_right - 2 * u_mid + u_left) / pow(inter_coord, 2) -
            (u_right - u_left) / (2 * inter_coord)) * inter_time + u_mid;
}

double transport_eq_angle(int k, int m, double u_left, double u_mid) {
    return (function_f(k - 1, m) - (u_mid - u_left) / inter_coord) * inter_time + u_mid;
}

int main(int argc, char *argv[]) {
    double start_time, end_time;
    int start, end, diff, rest;
    int size, rank;
    int i, j;

    double max_time = 1, max_coord = 1;
    int len_time, len_coord;
    if (argc < 3) {
        len_time = 10;
        len_coord = 1000;
    } else {
        len_time = atoi(argv[1]);
        len_coord = atoi(argv[2]);
    }
    double result[len_coord][len_time];
    double result_it[len_coord];
    inter_coord = max_coord / (len_coord - 1);
    inter_time = max_time / (len_time - 1);
    for (i = 0; i < len_coord; i++) {
        result[i][0] = function_coord(i);
    }
    for (i = 0; i < len_time; i++) {
        result[0][i] = function_time(i);
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Request req1, req2, req3, req4;
    MPI_Status status;
    start_time = MPI_Wtime();

    diff = len_coord / size;
    rest = len_coord % size;
    if (rank < rest) {
        diff++;
        start = rank * diff;
        end = start + diff - 1;
    } else {
        start = rank * diff + rest;
        end = start + diff - 1;
    }
    if (rank == 0) {
        if (size != 1) {
            for (i = 1; i < len_time; i++) {
                result[end][i] = transport_eq(i, end, result[end - 1][i - 1], result[end][i - 1],
                                              result[end + 1][i - 1]);
                MPI_Isend(&result[end][i], 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &req1);
                for (j = start + 1; j <= end - 1; j++) {
                    result[j][i] = transport_eq(i, start, result[j - 1][i - 1], result[j][i - 1], result[j + 1][i - 1]);
                }
                MPI_Recv(&result[end + 1][i], 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &status);
                MPI_Wait(&req1, &status);

//                for (j = end + 1; j < len_coord; j++) {
//                    MPI_Recv(&result[j][i], 1, MPI_DOUBLE, MPI_ANY_SOURCE, j, MPI_COMM_WORLD, &status);
//                }
            }
        } else {
            for (i = 1; i < len_time; i++) {
                result[end][i] = transport_eq_angle(i, end, result[end - 1][i - 1], result[end][i - 1]);
                for (j = start + 1; j <= end - 1; j++) {
                    result[j][i] = transport_eq(i, start, result[j - 1][i - 1], result[j][i - 1], result[j + 1][i - 1]);
                }
            }
        }
    } else if (rank == size - 1) {
        for (i = 1; i < len_time; i++) {
            result[start][i] = transport_eq(i, start, result[start - 1][i - 1], result[start][i - 1],
                                            result[start + 1][i - 1]);
            result[end][i] = transport_eq_angle(i, end, result[end - 1][i - 1], result[end][i - 1]);
            MPI_Isend(&result[start][i], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &req2);
            for (j = start + 1; j <= end - 1; j++) {
                result[j][i] = transport_eq(i, start, result[j - 1][i - 1], result[j][i - 1], result[j + 1][i - 1]);
            }
            MPI_Recv(&result[start - 1][i], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &status);
            MPI_Wait(&req2, &status);

//            for (j = start; j <= end; j++) {
//                MPI_Send(&result[j][i], 1, MPI_DOUBLE, 0, j, MPI_COMM_WORLD);
//            }
        }
    } else {
        for (i = 1; i < len_time; i++) {
            result[start][i] = transport_eq(i, start, result[start - 1][i - 1], result[start][i - 1],
                                            result[start + 1][i - 1]);
            result[end][i] = transport_eq(i, end, result[end - 1][i - 1], result[end][i - 1],
                                          result[end + 1][i - 1]);
            MPI_Isend(&result[start][i], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &req3);
            MPI_Isend(&result[end][i], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &req4);
            for (j = start + 1; j <= end - 1; j++) {
                result[j][i] = transport_eq(i, start, result[j - 1][i - 1], result[j][i - 1], result[j + 1][i - 1]);
            }
            MPI_Recv(&result[start - 1][i], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&result[end + 1][i], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &status);
            MPI_Wait(&req3, &status);
            MPI_Wait(&req4, &status);

//            for (j = start; j <= end; j++) {
//                MPI_Send(&result[j][i], 1, MPI_DOUBLE, 0, j, MPI_COMM_WORLD);
//            }
        }
    }

    end_time = MPI_Wtime();
    if (rank == 0) {
        for (j = end + 1; j < len_coord; j++) {
            MPI_Recv(&result[j][len_time - 1], 1, MPI_DOUBLE, MPI_ANY_SOURCE, j, MPI_COMM_WORLD, &status);
        }
        // GET the matrix
//        for (i = len_time - 1; i >= 0; i--) {
//            printf("| ");
//            for (j = 0; j < len_coord; j++) {
//                printf("%5.2lf | ", result[j][i]);
//            }
//            printf("\n");
//        }

        // GET the last string
        for (j = 0; j < len_coord; j++) {
            printf("x=%15.12lf --- u=%.10lf\n", j * inter_coord, result[j][len_time - 1]);
        }
        // GET the last element
//        printf("Result:\n\n");
//        printf("x=%15.12lf --- t=%15.12lf --- u=%21.10lf\n", max_coord, max_time, result[len_coord - 1][len_time - 1]);
//        printf("With %d; for %f\n\n", size, end_time - start_time);

        printf("In time %lf; with %d; for %f\n", max_time, size, end_time - start_time);
    }
    else {
        for (j = start; j <= end; j++) {
            MPI_Send(&result[j][len_time - 1], 1, MPI_DOUBLE, 0, j, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();

    return 0;
}