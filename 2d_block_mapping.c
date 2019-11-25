#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mpi.h"

/*
 * Name : Parallel Floyd Algorithm
 * Owner : Aloys Portafaix
 */

#define MASTER 0
#define SIZE 4
#define INF 9999

int min(int a, int b);
void generateMatrix(int arr[][SIZE]);
void printMatrix(int arr[][SIZE]);

int main(int argc, char *argv[]){
    int rank, num_processes, recv_data[SIZE];
//    int matrix[SIZE][SIZE] = {{0, 3,   INF, 7},
//                              {8, 0,   2, INF},
//                              {5, INF, 0,   1},
//                              {2, INF, INF, 0}};
    int matrix[SIZE][SIZE];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    if(rank == MASTER){
        generateMatrix(matrix);
        printMatrix(matrix);
    }

    MPI_Bcast(matrix, SIZE*SIZE, MPI_INT, MASTER, MPI_COMM_WORLD);

    // floyd algorithm
    int k, i;
    for(k = 0; k < SIZE; k++){
        // scatter matrix to all processors, each receives one row
        if(rank==0){
            MPI_Scatter(matrix, SIZE*SIZE/num_processes, MPI_INT, &recv_data, SIZE*SIZE/num_processes, MPI_INT, MASTER, MPI_COMM_WORLD);
        } else {
            MPI_Scatter(&matrix, 0, MPI_INT, &recv_data, SIZE*SIZE/num_processes, MPI_INT, MASTER, MPI_COMM_WORLD);
        }

        // update local row associated to processor except if it is the kth row or column
        if(rank != k) {
            for (i = 0; i < SIZE; i++) {
                if(i != k){
                    recv_data[i] = min(matrix[rank][i], matrix[rank][k] + matrix[k][i]);
                }
            }
        }

        // update matrix
        int *temp_mat = NULL;
        if(rank == MASTER){
            temp_mat = malloc(SIZE*SIZE*sizeof(int));
            MPI_Gather(recv_data, SIZE*SIZE/num_processes, MPI_INT, temp_mat, SIZE*SIZE/num_processes, MPI_INT, MASTER, MPI_COMM_WORLD);

            // update matrix with data received
            int row, col;
            int count = 0;
            for(row = 0; row < SIZE; row++){
                for(col = 0; col < SIZE; col++){
                     matrix[row][col] = temp_mat[count];
                     count++;
                }
            }
        }else{
            MPI_Gather(recv_data, SIZE*SIZE/num_processes, MPI_INT, temp_mat, SIZE*SIZE/num_processes, MPI_INT, MASTER, MPI_COMM_WORLD);
        }

        // broadcast changes to matrix
        MPI_Bcast(matrix, SIZE*SIZE, MPI_INT, MASTER, MPI_COMM_WORLD);
    }

    if(rank == MASTER) {
        printMatrix(matrix);
    }
    MPI_Finalize();
    return 0;
}

void generateMatrix(int arr[][SIZE]){
    int i,j;
    srandom(time(NULL));
    for(i = 0; i < SIZE; i++){
        for(j = 0; j < SIZE; j++){
            // no loops
            if(i == j){
                arr[i][j] = 0;
            }else {
                arr[i][j] = (int) random() % 11;
                // replace 0's and 10's with INFINITY
                if (arr[i][j] == 0 || arr[i][j] == 10) {
                    arr[i][j] = INF;
                }
            }
        }
    }
}

int min(int a, int b){
    if (a < b){
        return a;
    }
    return b;
}

void printMatrix(int arr[][SIZE]){
    int i, j;
    for(i = 0; i < SIZE; i++){
        for(j = 0; j < SIZE; j++){
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

