#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define M 10
#define N 10
#define Vleft 1.0
#define Vright 1.0
#define Vtop 1.0
#define Vbottom 1.0
#define Vinit 0.0
#define delta 0.1  // Time step size

// Function to print the V_Temperature
void printV_Temperature(double **V_Temperature, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%.2f\t", V_Temperature[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Calculate subregion size and boundaries
    int subregion_size = M / size;
    int subregion_start = rank * subregion_size;
    int subregion_end = (rank + 1) * subregion_size;

    // Declare and initialize the local V_Temperature array for each process
    double **V_Temperature_local = (double **)malloc(subregion_size * sizeof(double *));
    for (int i = 0; i < subregion_size; i++) {
        V_Temperature_local[i] = (double *)malloc(N * sizeof(double));
        for (int j = 0; j < N; j++) {
            if (subregion_start + i == 0)
                V_Temperature_local[i][j] = Vtop;
            else if (subregion_start + i == M - 1)
                V_Temperature_local[i][j] = Vbottom;
            else if (j == 0)
                V_Temperature_local[i][j] = Vleft;
            else if (j == N - 1)
                V_Temperature_local[i][j] = Vright;
            else
                V_Temperature_local[i][j] = Vinit;
        }
    }

    // Allocate separate arrays for sending and receiving ghost cells
    double *send_top = (double *)malloc(N * sizeof(double));
    double *recv_top = (double *)malloc(N * sizeof(double));
    double *send_bottom = (double *)malloc(N * sizeof(double));
    double *recv_bottom = (double *)malloc(N * sizeof(double));

    // Loop that simulates the dimension
    for (int k = 0; k < 100; k++) {
        // Copy boundary values to send buffers
        for (int j = 0; j < N; j++) {
            send_top[j] = V_Temperature_local[0][j];
            send_bottom[j] = V_Temperature_local[subregion_size - 1][j];
        }

        // Communicate ghost cells with neighbors using MPI
        if (rank > 0) {
            MPI_Send(send_top, N, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD);
            MPI_Recv(recv_top, N, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Update ghost cells with received values
            for (int j = 0; j < N; j++) {
                V_Temperature_local[0][j] = recv_top[j];
            }
        }

        if (rank < size - 1) {
            MPI_Send(send_bottom, N, MPI_DOUBLE, rank + 1, 1, MPI_COMM_WORLD);
            MPI_Recv(recv_bottom, N, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Update ghost cells with received values
            for (int j = 0; j < N; j++) {
                V_Temperature_local[subregion_size - 1][j] = recv_bottom[j];
            }
        }

        // Iterate over the interior of the local V_Temperature (excluding boundaries)
        for (int i = 1; i < subregion_size - 1; i++) {
            for (int j = 1; j < N - 1; j++) {
                // Updating each element using the 4-point stencil
                V_Temperature_local[i][j] = V_Temperature_local[i][j] + delta * (
                    (V_Temperature_local[i - 1][j] + V_Temperature_local[i + 1][j] + V_Temperature_local[i][j - 1] + V_Temperature_local[i][j + 1]) / 4.0 - V_Temperature_local[i][j]);
            }
        }

        // Print the V_Temperature every 5 timesteps
        //if (k % 5 == 0) {
          //  printf("Process %d V_Temperature after %d timesteps:\n", rank, k);
            //printV_Temperature(V_Temperature_local, subregion_size, N);
        //}
    }

    // Gather all subregions to process 0
    MPI_Gather(V_Temperature_local[0], subregion_size * N, MPI_DOUBLE, V_Temperature_local[0], subregion_size * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Print the final state of the V_Temperature for process 0
        printf("Final V_Temperature:\n");
        printV_Temperature(V_Temperature_local, M, N);
    }

    // Free allocated memory
    for (int i = 0; i < subregion_size; i++) {
        free(V_Temperature_local[i]);
    }
    free(V_Temperature_local);

    free(send_top);
    free(recv_top);
    free(send_bottom);
    free(recv_bottom);

    MPI_Finalize();

    return 0;
}

