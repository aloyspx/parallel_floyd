#include <stdlib.h>
#include <stdio.h>

#include "mpi.h"

void printProcMatrix(int recv_data[], int rank, int num_processes, int SIZE){
    int *temp_mat = NULL;

    if(rank == 0){
        temp_mat = malloc(SIZE*SIZE*sizeof(int));
        MPI_Gather(recv_data, SIZE*SIZE/num_processes, MPI_INT, temp_mat, SIZE*SIZE/num_processes, MPI_INT, 0, MPI_COMM_WORLD);
        int i, j;
        for(i = 0; i < SIZE; i++){
            for(j = 0; j < SIZE; j++){
                printf("%d ", temp_mat[i * SIZE + j]);
            }
            printf("\n");
        }
    }else{
        MPI_Gather(recv_data, SIZE*SIZE/num_processes, MPI_INT, temp_mat, SIZE*SIZE/num_processes, MPI_INT, 0, MPI_COMM_WORLD);
    }
    free(temp_mat);
}