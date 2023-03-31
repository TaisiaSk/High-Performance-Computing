#include <iostream>
#include <omp.h>

// mode == 0 - sequential
// mode == 1 - atomic
// mode == 2 - reduction

void sum(intptr_t n, char mode) {
    intptr_t sum = 0;
    intptr_t* A = new intptr_t[n];
    double start = 0;

    for (intptr_t i = 0; i < n; ++i)
        A[i] = i;

    if (mode == 2) {
        std::cout << "Reduction" << std::endl;
        start = omp_get_wtime();

#pragma omp parallel for reduction(+:sum)
        for (intptr_t i = 0; i < n; ++i)
            sum += A[i];
    }
    else if (mode == 1) {
        std::cout << "Atomic" << std::endl;
        start = omp_get_wtime();

#pragma omp parallel for
        for (intptr_t i = 0; i < n; ++i) {
#pragma omp atomic
            sum += A[i];
        }
    }
    else {
        std::cout << "Sequential" << std::endl;
        start = omp_get_wtime();

        for (intptr_t i = 0; i < n; ++i)
            sum += A[i];
    }

    double end = omp_get_wtime();

    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Time: " << end - start << std::endl;

    delete[] A;
}

int main() {
    std::cout << "Sum of numbers" << std::endl;
    intptr_t n = 1e6;
    
    sum(n, 0);
    sum(n, 1);
    sum(n, 2);

    return 0;
}
