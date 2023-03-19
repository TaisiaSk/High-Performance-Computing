#include <iostream>
#include <fstream>
#include <iomanip>
#include <omp.h>

int main(){
    std::cout << "Calculating PI" << std::endl;
    std::cout << "Result in \"output.txt\" file." << std::endl;
    std::ofstream outFile("output.txt");

    const intptr_t N = 12;
    const intptr_t N_TREADS[] = { 1, 2, 4, 8, 10, 12 };
    double x;
    double time[N];
    double result[N];

    for (intptr_t k = 0; k < N; ++k) {
        double pi = .0;
        intptr_t n = (k % 2 == 0) ? 1e2 : 1e6;
        intptr_t treads = N_TREADS[k / 2];

        double start = omp_get_wtime();

#pragma omp parallel for num_threads(treads) private(x), reduction(+:pi)
        for (intptr_t i = 0; i < n; ++i) {
            x = (i + 0.5) / n;
            pi += 4 / (1 + x * x);
        }

        pi /= n;

        double end = omp_get_wtime();
        time[k] = (end - start) * 1000;
        result[k] = pi;
    }   

    outFile << std::left << std::setw(8) << "Precision";
    outFile << std::left << "   " << std::setw(8) << "Treads";
    outFile << std::left << std::setw(8) << "Time";
    outFile << std::left << "   " << std::setw(8) << "PI" << std::endl;

    for (intptr_t i = 0; i < N; ++i) {
        outFile << std::setprecision(9) << std::left << std::setw(8) << (i % 2 == 0 ? 1e2 : 1e6);
        outFile << std::left << "   " << std::setw(8) << N_TREADS[i / 2];
        outFile << std::setprecision(9) << std::left << std::setw(8) << time[i];
        outFile << std::setprecision(9) << std::left << "   " << std::setw(8) << result[i] << std::endl;
    }

    return 0;
}
