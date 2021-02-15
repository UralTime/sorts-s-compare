#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

void swap (long long *a, long long *b) {
    long long tmp = *a;
    *a = *b;
    *b = tmp;
}

int compare1 (const void *x1, const void *x2) { // функция сравнения элементов массива для qsort по неубыванию
    long long diff = *(long long*)x1 - *(long long *)x2;
    if (diff > 0)
        return 1;
    if (diff < 0)
        return -1;
    return 0;
}

int compare2 (const void *x1, const void *x2) { // функция сравнения элементов массива для qsort по невозрастанию
    return compare1(x2, x1);
}

void PsevdoRandom (long long *a, int n, int param) { // генерация массива
    srand(time(NULL));
    for (int i = 0; i < n; i++)
        a[i] = rand() * rand() * rand() * rand() * rand();
    if (param == 1) // нужен отсортированный по неубыванию
        qsort(a, n, sizeof(long long), compare1);
    else if (param == 2) // нужен отсортированный по невозрастанию
        qsort(a, n, sizeof(long long), compare2);
}

unsigned long long cntcompares = 0, cntmoves = 0; // было сказано, что можно глобальные переменные (удобно для рекурсивного qsort)

int AbsCompare (long long x1, long long x2, unsigned test) {
    cntcompares += test;
    if (x1 == LLONG_MIN) // самый большой модуль
        if (x2 == LLONG_MIN)
            return 0; // равны, а не >
        else
            return 1; // x1 > x2
    else
        if (x2 == LLONG_MIN)
            return 0; // x1 < x2, так как у x2 самый большой модуль
    long long absdiff = ((x1 >= 0)? x1: -x1) - ((x2 >= 0)? x2: -x2);
    return (absdiff > 0)? 1: 0;
}

void BubbleSort (long long *a, int n) {
    for (int i = 1; i < n; i++)
        for (int j = n - 1; j >= i; --j) //самый маленький элемент спускается вниз
            if (AbsCompare(a[j - 1], a[j], 1)) {
                swap(a + j - 1, a + j);
                cntmoves++;
            }
            // в пузырьковой можно и заранее подсчитать число сравнений
}

void qSort (long long *a, int left, int right) {
// comp -- компаранд, от его хорошего выбора зависит скорость сортировки
    int i = left, j = right; // была бага с unsigned из-за строчки 43 j > left при left = 0
    long long comp = a[(left + right) / 2]; // можно брать a[left + rand() % (j - i + 1)], но для надёжности оставим так
    do {
        while (AbsCompare(comp, a[i], 1) && i < right)
            i++; // теперь левая часть меньше comp
        while (AbsCompare(a[j], comp, 1) && j > left)
            j--; // теперь правая часть больше comp
        if (i <= j) { // есть элементы, на которых порядок нарушился
            swap(a + i, a + j);
            cntmoves++;
            i++, j--;
        }
    } while (i <= j);
    if (left < j)
        qSort(a, left, j); // сортируем левую часть
    if (i < right)
        qSort(a, i, right); // сортируем правую часть
}

void QuickSort (long long *a, int n) {
    qSort(a, 0, n - 1); // была бага с unsigned при n == 1
}

int TestFailed (long long *a, int n) {
    for (int i = 1; i < n; i++)
        if (AbsCompare(a[i - 1], a[i], 0)) // неупорядочено по неубыванию модулей
            return 1;
    return 0;
}

int main (void) {
    int n, param;
    printf("Please enter a size of array...\n");
    scanf("%u", &n);
    printf("If you want data ordered ascending - press 1, reverse (descending) - press 2, random - press 3 or 4\n");
    scanf("%u", &param);
    long long *a = malloc(n * sizeof(long long));
    PsevdoRandom(a, n, param);
    QuickSort(a, n);
    if (TestFailed(a, n))
        printf("Your sort is failed! Please check QuickSort's realisation!\n");
    else
        printf("You're god damn right! QuickSort done %llu compares and %llu moves\n", cntcompares, cntmoves);
    cntcompares = 0; cntmoves = 0;
    PsevdoRandom(a, n, param);
    BubbleSort(a, n);
    if (TestFailed(a, n))
        printf("Your sort is failed! Please check BubbleSort's realisation!\n");
    else
        printf("You're god damn right! BubbleSort done %llu compares and %llu moves\n", cntcompares, cntmoves);
    return 0;
}
