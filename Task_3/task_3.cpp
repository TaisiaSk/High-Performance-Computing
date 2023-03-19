#include <iostream>
#include <random>
#include <omp.h>

int main(){
    std::cout << "Matrix Multiplication" << std::endl;

    const intptr_t MIN_VALUE = -100;
    const intptr_t MAX_VALUE = 100;
    srand(time(NULL));

    intptr_t n = 1e3;
    intptr_t** A = new intptr_t * [n];
    intptr_t** B = new intptr_t * [n];
    intptr_t** C = new intptr_t * [n];

    for (intptr_t i = 0; i < n; ++i) {
        A[i] = new intptr_t[n];
        B[i] = new intptr_t[n];
        C[i] = new intptr_t[n];
    }    

    for (intptr_t i = 0; i < n; ++i)
        for (intptr_t j = 0; j < n; ++j) {
            A[i][j] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
            B[i][j] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
            C[i][j] = 0;
        }

    double start = omp_get_wtime();
#pragma omp parallel for num_threads(4)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    double end = omp_get_wtime();

    std::cout << "Time in ms: " << (end - start) * 1000 << std::endl;
    
    return 0;
}
