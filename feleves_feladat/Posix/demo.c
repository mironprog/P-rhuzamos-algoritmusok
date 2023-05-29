#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MATRIX_SIZE 3
#define NUM_THREADS 2

int matrix_a[MATRIX_SIZE][MATRIX_SIZE];
int matrix_b[MATRIX_SIZE][MATRIX_SIZE];
int matrix_result[MATRIX_SIZE][MATRIX_SIZE];

typedef struct {
    int row_start;
    int row_end;
} ThreadArgs;

void* multiply(void* thread_args) {
    ThreadArgs* args = (ThreadArgs*)thread_args;

    for (int i = args->row_start; i < args->row_end; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix_result[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                matrix_result[i][j] += matrix_a[i][k] * matrix_b[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadArgs thread_args[NUM_THREADS];

    // Initialize the matrices
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix_a[i][j] = i + j;
            matrix_b[i][j] = i - j;
        }
    }

    // Divide work among threads
    int rows_per_thread = MATRIX_SIZE / NUM_THREADS;
    int row_start = 0;
    int row_end = rows_per_thread;

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i].row_start = row_start;
        thread_args[i].row_end = row_end;

        pthread_create(&threads[i], NULL, multiply, (void*)&thread_args[i]);

        row_start = row_end;
        row_end += rows_per_thread;
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the result matrix
    printf("Result:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", matrix_result[i][j]);
        }
        printf("\n");
    }

    return 0;
}
