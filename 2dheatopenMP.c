#include <stdio.h>
#include <stdlib.h>
//#include "omp.h"

#define M 10
#define N 10
#define Vleft 1.0
#define Vright 1.0
#define Vtop 1.0
#define Vbottom 1.0
#define Vinit 0.0
#define delta 0.1  // Time step size

// Function to print the V_Temperature
void printV_Temperature(double **V_Temperature) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f\t", V_Temperature[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    //omp_set_num_threads(4); // Set the desired number of thread
    setenv("OMP_NUM_THREADS", "4", 1); // Set the desired number of threads. the one argument means it will override anyone that previously exist.
    

    // Declare and initialize the V_Temperature using malloc
    double **V_Temperature = (double **)malloc(M * sizeof(double *));
    for (int i = 0; i < M; i++) {
        V_Temperature[i] = (double *)malloc(N * sizeof(double));
    }

    // Initialize the V_Temperature based on boundary conditions and initial values
    #pragma omp parallel for collapse(2)
    //The collapse(2) clause is used to collapse the nested loops into a single loop for better parallelization
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (j == 0)
                V_Temperature[i][j] = Vleft;
            else if (j == N - 1)
                V_Temperature[i][j] = Vright;
            else if (i == 0)
                V_Temperature[i][j] = Vtop;
            else if (i == M - 1)
                V_Temperature[i][j] = Vbottom;
            else
                V_Temperature[i][j] = Vinit;
        }
    }

    // Print the initial state of the V_Temperature
    printf("Initial V_Temperature:\n");
    printV_Temperature(V_Temperature);

    // Loop that simulates the dimension
    for (int k = 0; k < 10; k++) {
        // Iterate over the interior of the V_Temperature (excluding boundaries)
        #pragma omp parallel for collapse(2)
        for (int i = 1; i < M - 1; i++) {
            for (int j = 1; j < N - 1; j++) {
                // Updating each element using the 4-point stencil
                V_Temperature[i][j] = V_Temperature[i][j] + delta * (
                (V_Temperature[i - 1][j] + V_Temperature[i + 1][j] + V_Temperature[i][j - 1] + V_Temperature[i][j + 1]) / 4.0 - V_Temperature[i][j]);
            }
        }

        // Print the V_Temperature every 5 timesteps
        if (k % 5 == 0) {
            printf("V_Temperature after %d timesteps:\n", k);
            printV_Temperature(V_Temperature);
        }
    }

    // Print the final state of the V_Temperature
    printf("Final V_Temperature:\n");
    printV_Temperature(V_Temperature);

    // Free the allocated memory
    for (int i = 0; i < M; i++) {
        free(V_Temperature[i]);
    }
    free(V_Temperature);

    return 0;
}

