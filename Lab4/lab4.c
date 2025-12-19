#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"
#include "functions.h"
// #include "functions.h"

// void swap(int *a, int *b)
//     {
//         int temp;
//         temp = *a;
//         *a = *b;
//         *b = temp;
//     }

MU_TEST(test_swap_positive_numbers) {
    int a = 10, b = 20;
    swap(&a, &b);
    mu_check(a == 20 && b == 10);
}

MU_TEST(test_swap_negative_numbers) {
    int a = -100, b = -200;
    swap(&a, &b);
    mu_check(a == -200 && b == -100);
}

MU_TEST(test_swap_zero_and_positive) {
    int a = 0, b = 42;
    swap(&a, &b);
    mu_check(a == 42 && b == 0);
}

MU_TEST(test_swap_zero_and_negative) {
    int a = 0, b = -777;
    swap(&a, &b);
    mu_check(a == -777 && b == 0);
}

MU_TEST(test_swap_identical_values) {
    int a = 555, b = 555;
    swap(&a, &b);
    mu_check(a == 555 && b == 555);
}

MU_TEST(test_swap_same_pointer) {
    int x = 12345;
    swap(&x, &x);
    mu_check(x == 12345);
}

MU_TEST(test_swap_int_max_and_int_min) {
    int a = INT_MAX, b = INT_MIN;
    swap(&a, &b);
    mu_check(a == INT_MIN && b == INT_MAX);
}

MU_TEST(test_swap_int_min_and_int_max) {
    int a = INT_MIN, b = INT_MAX;
    swap(&a, &b);
    mu_check(a == INT_MAX && b == INT_MIN);
}

MU_TEST(test_swap_with_zero) {
    int a = 0, b = 0;
    swap(&a, &b);
    mu_check(a == 0 && b == 0);
}

MU_TEST(test_swap_large_positive_numbers) {
    int a = 1000000000, b = 2000000000;
    swap(&a, &b);
    mu_check(a == 2000000000 && b == 1000000000);
}

MU_TEST(test_swap_modifies_original_variables) {
    int x = 7, y = 8;
    swap(&x, &y);
    mu_check(x == 8 && y == 7);
}

// MU_TEST(test_swap_different_types_wont_compile) {
//     // short s = 5;
//     // long long ll = 9999999999LL;
//     // swap(&s, &ll); 
//     mu_check(1);
// }


MU_TEST_SUITE(swap_test_suite) {
    MU_RUN_TEST(test_swap_positive_numbers);
    MU_RUN_TEST(test_swap_negative_numbers);
    MU_RUN_TEST(test_swap_zero_and_positive);
    MU_RUN_TEST(test_swap_zero_and_negative);
    MU_RUN_TEST(test_swap_identical_values);
    MU_RUN_TEST(test_swap_same_pointer);
    MU_RUN_TEST(test_swap_int_max_and_int_min);
    MU_RUN_TEST(test_swap_int_min_and_int_max);
    MU_RUN_TEST(test_swap_with_zero);
    MU_RUN_TEST(test_swap_large_positive_numbers);
    MU_RUN_TEST(test_swap_modifies_original_variables);
    MU_REPORT();
}

MU_TEST(strcomp_123_231){
    char a[] = "123";
    char b[] = "123";
    mu_check(myStrCmp(a, b) == strcmp(a, b));
}

MU_TEST(strcomp_a_b){
    char a[] = "a";
    char b[] = "b";
    mu_check(myStrCmp(a, b) == strcmp(a, b));
}

MU_TEST_SUITE(myStringCompare) { 
    MU_RUN_TEST(strcomp_123_231);
    // MU_RUN_TEST(strcomp_a_b);
    MU_REPORT();
}

int main(){
    MU_RUN_SUITE(swap_test_suite);
    // Задание 1 (творческое). swap
    // Напишите функцию swap(int *i, int *j), которая меняет значения двух ячеек местами.
    // Напишите как можно больше тестов для этой функции (не меньше 10)
    // На звёздочку: используйте фреймворк наподобие MinUnit https://github.com/siu/minunit
    

    // Задание 2. Указатели на массивы в качестве аргументов.
    // Напишите два варианта функции поиска минимального элемента 
    // 2a. во встроенном двухмерном массиве
    // 2b. в динамическом двухмерном массиве (обе размерности вычисляются)

    // Например:
    // int ar[N][M] = {список значений};
    // int res = myMin(ar, ... наверняка понадобится передать ещё какие-нибудь данные);
    #define N 3
    #define M 4
    int ar[N][M] = {{1, 5, 2, 4},
                    {2, 4, 7, 8},
                    {0, -5, 2, 4}};
    int ** dAr = malloc(sizeof(int *) * N);
    for (int i = 0; i < N; i++){
        dAr[i] = malloc(sizeof(int) * M);
        for (int j = 0; j < M; j++){
            dAr[i][j] = rand() % 30;
            printf("%d ", dAr[i][j]);
        }
        printf("\n");
    }
    printf("MyMinStatic result %d\n", myMinStatic(ar, N, M));
    printf("MyMinDynamic result %d\n", myMinDynamic(dAr, N, M));
    // printf("%d", myStrCmp(&"1234", &"4321"));

    // Задание 3. Напишите функцию сравнения двух строк - myStrCmp():
    // Функция должна принимать два указателя на сравниваемые
    // строки (возможно, понадобится сказать компилятору, что функция
    // не имеет права модифицировать содержимое строк)
    // Функция должна возвращать отрицательное значение, если
    // первая строка лексиграфически меньше второй, 0 - если они равны
    // и положительное значение, если первая строка больше второй.
    // Вспомните, что признаком конца строки является нулевой байт.
    // MU_RUN_SUITE(myStringCompare);
    MU_RUN_SUITE(myStringCompare);

    // Задание 4. Создайте одномерный динамический массив, заполните его значениями.
    // Напишите функцию, которая добавляет в массив новое значение только при условии,
    // что такого значения в массиве еще нет. Подсказка: при этом размер
    // массива должен увеличиться!
    // Для проверки напишите функцию, которая выводит значения всех элементов на экран
    int sizeSecondArr = 10;
    int * secondDynArr = calloc(sizeSecondArr,sizeof(int));
    for (int i = 0; i < sizeSecondArr; i++){
        secondDynArr[i] = rand() % 20;
    }
    antiDuplicateAppend(secondDynArr, &sizeSecondArr);
    printIntArr(secondDynArr, sizeSecondArr );
    // Задание 5. Рекурсивные функции.
    // 5a. Напишите рекурсивную функцию вычисления суммы первых N натуральных чисел
    printf("%d\n", sumRecursive(6));
    // 5б. Напишите рекурсивную функцию для вычисления N-го числа Фибоначчи
    printf("%d\n", fibRecursive(10));

    // Задание 6а. Функции с переменным числом параметров.
    // Напишите функцию varArgs, не использующую макросы,
    // которая принимает переменное число аргументов (см. подсказку внизу кода задания)
    // типа int и выводит на печать число принятых параметров и их значения.
    // Признаком конца списка параметров является нулевое значение.

    int args[] = {5, 11, 4, 7, -1};
    varArgs(args[0], 0);
    varArgs(args[0], args[1], 0);
    varArgs(args[0], args[1], args[2], args[3], args[4], 0);
    

    // Задание 6б. Модифицируйте функцию 5а с помощью макросов
    //  va_start, va_arg, va_end
    varArgs_va(args[0], args[1], args[2], args[3], args[4], 0);

    // Задание 7. Возвращение адреса.
    // Напишите функцию, которая находит минимальное значение в массиве,
    // таким образом, чтобы ее вызов можно было использовать слева от знака
    // равенства: *myMin(параметры) = 0;
    *myMin(secondDynArr, sizeSecondArr * 2) = 0;

    return 0;
}

//Подсказка-заготовка для задания 6а
// рекомендуется вынести функцию в отдельный файл с соответствующим заголовочным файлом
/*
void varArgs(int arg1,...)
{
	int nArgs = 0;	//счетчик элементов
	//Объявите указатель на int и инициализируйте его адресом первого аргумента
	
	//Пока не достигнут конец списка:
	// а) печать значения очередного аргумента
	// б) модификация указателя (он должен указывать на следующий аргумент списка)
	// в) увеличение счётчика элементов

	//Печать числа элементов

}
*/
