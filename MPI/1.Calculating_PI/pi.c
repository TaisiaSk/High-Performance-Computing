#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define PI 3.14159265358979323846

int main(int argc, char **argv)
{
    int rank, size;
    int i;
    double pi = .0, global_pi = .0, x;
    const int n = 1e6;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    for (i = rank; i < n; i += size)
    {
        x = (i + 0.5) / n;
        pi += 4 / (1 + x * x);
    }

    MPI_Reduce(&pi, &global_pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0)
    {
        global_pi /= n;

        printf("Precision: %u \n", n);
        printf("PI: %9.8f \n", PI);
        printf("Calculated: %9.8f \n", global_pi);
        printf("Difference: %e \n", PI - global_pi);
        printf("Time(ms): %f \n", (end - start) * 1000);
    }

    MPI_Finalize();
    return 0;
}
