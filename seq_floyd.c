#include <stdio.h>

#define SIZE 4
#define INF 9999

int min(int a, int b);
void serialFloydAlgo(int arr[][SIZE]);
void printResult(int arr[][SIZE]);

int main() {
    int arr[SIZE][SIZE] = {{0,3,INF,7},{8,0,2,INF},{5,INF,0,1}, {2,INF,INF,0}};
    serialFloydAlgo(arr);
}

void serialFloydAlgo(int arr[][SIZE]){
    int i, j, k;

    for(k = 0; k < SIZE; k++){
        for(i = 0; i < SIZE; i++){
            for(j = 0; j < SIZE; j++){
                arr[i][j] = min(arr[i][j], arr[i][k] + arr[k][j]);
            }
        }
    }
    printResult(arr);
}

int min(int a, int b){
    if(a < b){
        return a;
    }else{
        return b;
    }
}

void printResult(int arr[][SIZE]){
    int i, j;
    for(i = 0; i < SIZE; i++){
        for(j = 0; j < SIZE; j++){
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}