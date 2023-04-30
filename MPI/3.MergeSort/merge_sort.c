#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "mpi.h"

#define N 2048

void merge(int *arr, int l, int m, int r);
void mergeSort(int *arr, int l, int r);
bool checkSort(int *arr, int size);
void printArray(int *arr, int size);

int main(int argc, char **argv)
{
    int size, rank, i, sub_size;
    double start, end;
    int *sub_array;
    int *sorted_array;
    int *array = (int *)malloc(N * sizeof(int));

    srandom(time(NULL));
    for (i = 0; i < N; ++i)
    {
        array[i] = random() % 200 - 100;
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    sub_size = N / size;

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    sub_array = (int *)malloc(sub_size * sizeof(int));
    MPI_Scatter(array, sub_size, MPI_INT, sub_array, sub_size, MPI_INT, 0, MPI_COMM_WORLD);

    mergeSort(sub_array, 0, sub_size - 1);

    if (rank == 0)
    {
        sorted_array = (int *)malloc(N * sizeof(int));
    }

    MPI_Gather(sub_array, sub_size, MPI_INT, sorted_array, sub_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        mergeSort(sorted_array, 0, N - 1);
        end = MPI_Wtime();

        printf("Merge Sort\n");

        printf("Original Array:\n");
        printArray(array, N);

        printf("Sorted Array:\n");
        printArray(sorted_array, N);

        if (checkSort(sorted_array, N))
        {
            printf("Correct result\n");
            printf("\nTime(ms): %f \n", (end - start) * 1000);
        }
        else
        {
            printf("Uncorrect result\n");
        }

        free(sorted_array);
    }

    free(array);
    free(sub_array);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
}

void merge(int *arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int *left = (int *)malloc(n1 * sizeof(int));
    int *right = (int *)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        left[i] = arr[l + i];

    for (j = 0; j < n2; j++)
        right[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2)
    {
        if (left[i] <= right[j])
        {
            arr[k] = left[i];
            i++;
        }
        else
        {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = left[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = right[j];
        j++;
        k++;
    }

    free(left);
    free(right);
}

void mergeSort(int *arr, int l, int r)
{
    if (l >= r)
        return;

    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

bool checkSort(int *arr, int size)
{
    for (int i = 0; i < size - 1; i++)
        if (arr[i] > arr[i + 1])
            return 0;

    return 1;
}

void printArray(int *arr, int size)
{
    int i;

    for (i = 0; i < size; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}