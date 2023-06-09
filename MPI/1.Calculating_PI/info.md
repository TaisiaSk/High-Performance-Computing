# MPI. Задание 1

### MPI-программа, которая вычисляет число 𝜋 с точностью до N знаков после запятой

---

В начале программы происходит инициализация MPI, определение номера процесса и количества процессов, участвующих в вычислениях. Далее, каждый процесс вычисляет свою часть значения числа 𝜋.

В конце каждый процесс отправляет свое значение 𝜋 в процесс с номером 0, используя функцию MPI_Reduce для выполнения операции суммирования. На процессе 0 происходит вычисление общего значения числа 𝜋.

Значение числа 𝜋 вычисляется с использованием формулы:

$\pi=(\frac{4}{1+x_1^2}+\frac{4}{1+x_0^2}+...+\frac{4}{1+x_{N-1}^2})\times\frac{1}{N}$,

где $x_i=(i+0.5)\times\frac{1}{N}, i=\overline{1, N-1}$.

N - точность вычисления.

---

Результаты работы с разным количеством процессов представлены в таблице:

| Число процессов | Время (мс) |
| :-------------: | :--------: |
|       16        |    3.56    |
|       32        |    7.84    |
|       64        |   54.04    |
