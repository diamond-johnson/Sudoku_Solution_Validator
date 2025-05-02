#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define N 9

int sudoku[N][N] = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
};

int results[27] = {0};

typedef struct {
    int row;
    int col;
    int index;
} parameters;

//check row validity
void* check_row(void* param) {
    parameters* p = (parameters*) param;
    int row = p->row;
    int nums[N+1] = {0};
    for (int i = 0; i < N; i++) {
        int val = sudoku[row][i];
        if (val < 1 || val > 9 || nums[val]) {
            results[p->index] = 0;
            pthread_exit(NULL);
        }
        nums[val] = 1;
    }
    results[p->index] = 1;
    pthread_exit(NULL);
}

//check column validity
void* check_col(void* param) {
    parameters* p = (parameters*) param;
    int col = p->col;
    int nums[N+1] = {0};
    for (int i = 0; i < N; i++) {
        int val = sudoku[i][col];
        if (val < 1 || val > 9 || nums[val]) {
            results[p->index] = 0;
            pthread_exit(NULL);
        }
        nums[val] = 1;
    }
    results[p->index] = 1;
    pthread_exit(NULL);
}

//check subgrid validity
void* check_subgrid(void* param) {
    parameters* p = (parameters*) param;
    int start_row = p->row;
    int start_col = p->col;
    int nums[N+1] = {0};
    for (int i = start_row; i < start_row + 3; i++) {
        for (int j = start_col; j < start_col + 3; j++) {
            int val = sudoku[i][j];
            if (val < 1 || val > 9 || nums[val]) {
                results[p->index] = 0;
                pthread_exit(NULL);
            }
            nums[val] = 1;
        }
    }
    results[p->index] = 1;
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[27];
    parameters params[27];
    int thread_index = 0;

    for (int i = 0; i < N; i++) {
        params[thread_index].row = i;
        params[thread_index].col = 0;
        params[thread_index].index = thread_index;
        pthread_create(&threads[thread_index], NULL, check_row, (void*) &params[thread_index]);
        thread_index++;
    }

    for (int i = 0; i < N; i++) {
        params[thread_index].row = 0;
        params[thread_index].col = i;
        params[thread_index].index = thread_index;
        pthread_create(&threads[thread_index], NULL, check_col, (void*) &params[thread_index]);
        thread_index++;
    }

    for (int i = 0; i < N; i += 3) {
        for (int j = 0; j < N; j += 3) {
            params[thread_index].row = i;
            params[thread_index].col = j;
            params[thread_index].index = thread_index;
            pthread_create(&threads[thread_index], NULL, check_subgrid, (void*) &params[thread_index]);
            thread_index++;
        }
    }

    for (int i = 0; i < 27; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < 27; i++) {
        if (results[i] == 0) {
            printf("Sudoku is NOT valid.\n");
            return 0;
        }
    }

    printf("Sudoku IS valid!\n");
    return 0;
}
