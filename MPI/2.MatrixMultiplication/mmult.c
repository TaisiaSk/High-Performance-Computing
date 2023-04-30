#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "mpi.h"

#define N 512

void printMatrix(int matrix[N][N]);
bool checkResult(int A[N][N], int B[N][N], int C[N][N]);

int main(int argc, char **argv)
{
    int size, rank;
    MPI_Status status;
    int tasks, rows, offset, source, dest;
    double start, end;
    int C[N][N], A[N][N], B[N][N];

    srand(time(NULL));

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
        {
            A[i][j] = random() % 20 - 10;
            B[i][j] = random() % 20 - 10;
        }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
    {
        start = MPI_Wtime();

        rows = N / size;
        offset = rows;

        for (dest = 1; dest < size; ++dest)
        {
            MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&A[offset][0], rows * N, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&B, N * N, MPI_INT, dest, 1, MPI_COMM_WORLD);

            offset = offset + rows;
        }

        for (int k = 0; k < N; ++k)
            for (int i = 0; i < rows; ++i)
            {
                C[i][k] = 0;
                for (int j = 0; j < N; ++j)
                    C[i][k] = C[i][k] + A[i][j] * B[j][k];
            }

        for (source = 1; source < size; ++source)
        {
            MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&C[offset][0], rows * N, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
        }

        end = MPI_Wtime();

        printf("Matrix Multiplication: A*B=C\n");

        // -------------- Uncomment to output matrices --------------

        // printf("Matrix A\n");
        // printMatrix(A);

        // printf("\nMatrix B\n");
        // printMatrix(B);

        // printf("\nResult matrix C\n");
        // printMatrix(C);

        printf("\nTime(ms): %f \n", (end - start) * 1000);

        if (checkResult(A, B, C))
        {
            printf("Correct result\n");
        }
        else
        {
            printf("Uncorrect result\n");
        }
    }
    else
    {
        source = 0;

        MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&A, rows * N, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&B, N * N, MPI_INT, source, 1, MPI_COMM_WORLD, &status);

        for (int k = 0; k < N; ++k)
            for (int i = 0; i < rows; ++i)
            {
                C[i][k] = 0;
                for (int j = 0; j < N; ++j)
                    C[i][k] = C[i][k] + A[i][j] * B[j][k];
            }

        MPI_Send(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD);
        MPI_Send(&C, rows * N, MPI_INT, source, 2, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
}

void printMatrix(int matrix[N][N])
{
    int i, j;

    for (i = 0; i < N; ++i)
    {
        for (j = 0; j < N; ++j)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

bool checkResult(int A[N][N], int B[N][N], int C[N][N])
{
    int i, j, k, D[N][N];

    for (i = 0; i < N; ++i)
        for (j = 0; j < N; ++j)
        {
            D[i][j] = 0;

            for (k = 0; k < N; ++k)
            {
                D[i][j] += A[i][k] * B[k][j];
            }

            if (D[i][j] != C[i][j])
            {
                return 0;
            }
        }

    return 1;
}
