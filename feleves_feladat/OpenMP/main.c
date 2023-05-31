#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define DATASET_SIZE 1000000

// Function to calculate the average
double calculate_average(double* dataset, int size) {
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < size; i++) {
        sum += dataset[i];
    }
    return sum / size;
}

// Function to calculate the median
double calculate_median(double* dataset, int size) {
    // Sort the dataset 
    // bubble sort
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (dataset[j] > dataset[j + 1]) {
                double temp = dataset[j];
                dataset[j] = dataset[j + 1];
                dataset[j + 1] = temp;
            }
        }
    }

    // Calculate the median
    if (size % 2 == 0) {
        return (dataset[size / 2 - 1] + dataset[size / 2]) / 2.0;
    } else {
        return dataset[size / 2];
    }
}

// Function to calculate the standard deviation
double calculate_standard_deviation(double* dataset, int size, double average) {
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < size; i++) {
        double diff = dataset[i] - average;
        sum += diff * diff;
    }
    double variance = sum / size;
    return sqrt(variance);
}

int main() {
    double dataset[DATASET_SIZE];

    // Initialize the dataset with random values
    for (int i = 0; i < DATASET_SIZE; i++) {
        dataset[i] = (double)rand() / RAND_MAX;
    }

    double start_time = omp_get_wtime();

    // Calculate the average
    double average = calculate_average(dataset, DATASET_SIZE);

    // Calculate the median
    double median = calculate_median(dataset, DATASET_SIZE);

    // Calculate the standard deviation
    double deviation = calculate_standard_deviation(dataset, DATASET_SIZE, average);

    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;

    // Print the results
    printf("Average: %f\n", average);
    printf("Median: %f\n", median);
    printf("Standard Deviation: %f\n", deviation);
    printf("Elapsed Time: %f seconds\n", elapsed_time);

    return 0;
}
