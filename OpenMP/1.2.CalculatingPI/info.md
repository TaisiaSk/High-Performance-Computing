# OpenMP. Задание 1, задание 2

### OpenMP-программа, которая вычисляет число 𝜋 с точностью до N знаков после запятой.

---

Программа вычисляет число 𝜋 с использованием формулы

$\pi=(\frac{4}{1+x_1^2}+\frac{4}{1+x_0^2}+...+\frac{4}{1+x_{N-1}^2})\times\frac{1}{N}$,

где $x_i=(i+0.5)\times\frac{1}{N}, i=\overline{1, N-1}$.

N - точность вычисления.

Также проведено исследование масштабируемости с разными значениями точности вычисления и количества потоков. Результаты работы представлены в файле "output.txt".
