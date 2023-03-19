#include <iostream>
#include <random>
#include <omp.h>

void merge(intptr_t* arr, const intptr_t l, const intptr_t m, const intptr_t r) {
    const intptr_t N1 = m - l + 1;
    const intptr_t N2 = r - m;

    intptr_t* left = new intptr_t[N1];
    intptr_t* right = new intptr_t[N2];

    for (intptr_t i = 0; i < N1; i++) {
        left[i] = arr[l + i];
    }

    for (intptr_t j = 0; j < N2; j++) {
        right[j] = arr[m + 1 + j];
    }

    intptr_t i = 0, j = 0, k = l;

    while (i < N1 && j < N2) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        }
        else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < N1) {
        arr[k] = left[i];
        i++;
        k++;
    }

    while (j < N2) {
        arr[k] = right[j];
        j++;
        k++;
    }

    delete[] left;
    delete[] right;
}

void mergeSort(intptr_t* arr, const intptr_t l, const intptr_t r) {
    if (l >= r)
	return;

    intptr_t m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

void parallelMergeSort(intptr_t* arr, const intptr_t l, const intptr_t r) {
    if (l >= r)
	return;

    intptr_t m = l + (r - l) / 2;

#pragma omp task
     parallelMergeSort(arr, l, m);
#pragma omp task
     parallelMergeSort(arr, m + 1, r);

     merge(arr, l, m, r);
}

int main() {
    std::cout << "Merge Sort" << std::endl;

    const intptr_t N = 1e8;
    const intptr_t MIN_VALUE = -100;
    const intptr_t MAX_VALUE = 100;
    srand(time(NULL));

    intptr_t* arr1 = new intptr_t[N];
    intptr_t* arr2 = new intptr_t[N];
    for (intptr_t i = 0; i < N; ++i) {
        arr1[i] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
	arr2[i] = arr1[i];
    }

    std::cout << "Sequential" << std::endl;
    double start1 = omp_get_wtime();
    mergeSort(arr1, 0, N - 1);
    double end1 = omp_get_wtime();
    std::cout << "Time in ms: " << (end1 - start1) * 1000 << std::endl;

    std::cout << "Parallel" << std::endl;
    double start2 = omp_get_wtime();
#pragma omp parallel
    {
    #pragma omp single
    parallelMergeSort(arr2, 0, N - 1);
    }
    double end2 = omp_get_wtime();
    std::cout << "Time in ms: " << (end2 - start2) * 1000 << std::endl;

    delete[] arr1;
    delete[] arr2;

    return 0;
}
