#pragma once
#include "functions.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
void swap(int *a, int *b)
    {
        int temp;
        temp = *a;
        *a = *b;
        *b = temp;
    }

int myMinStatic(int ar[][M], int rows, int cols) {
    int min = INT_MAX;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (ar[i][j] < min) {
                min = ar[i][j];
            }
        }
    }
    return min;
}

int myMinDynamic(int **ar, int rows, int cols) {
    int min = INT_MAX;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (ar[i][j] < min) {
                min = ar[i][j];
            }
        }
    }
    return min;
}

int myStrCmp(const char *a, const char *b) {
    int i = 0;
    while (1) {
        if (a[i] == '\0' && b[i] == '\0') {
            return 0;
        }
        if (a[i] == '\0') {
            return -1;
        }
        if (b[i] == '\0') {
            return 1;
        }
        if (a[i] < b[i]) {
            return -1;
        }
        if (a[i] > b[i]) {
            return 1;
        }
        i++;
    }
}

void printIntArr(int * arr, size_t size){
    for (int i = 0; i < size; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int fibRecursive(int n){
    if (n == 0 || n == 1) return n;
    return fibRecursive(n-1) + fibRecursive(n-2);
}

int sumRecursive(int n){
    if (n == 1) return n;
    return sumRecursive(n-1) + n;
}

void varArgs(int argList, ...){
    int * argsArr = &argList;
    int argsAmount = 0;
    // int argsIndex = 0;
    while (*argsArr != 0){
        printf("%d, ", *argsArr);
        argsAmount += 1;
        argsArr += 1;
        // argsIndex += 1;
    }
    printf("\nArgs amount: %d\n", argsAmount);
}

int* myMin(int *arr, int size) {
    if (arr == NULL || size <= 0) {
        return NULL;
    }
    int *min_ptr = arr;
    for (int i = 1; i < size; i++) {
        if (arr[i] < *min_ptr) {
            min_ptr = &arr[i];
        }
    }
    return min_ptr;
}

void varArgs_va(int first, ...) {
    va_list args;
    va_start(args, first);

    int count = 0;
    int value = first;

    while (value != 0) {
        if (count > 0) printf(", ");
        printf("%d", value);
        count++;
        value = va_arg(args, int);
    }

    va_end(args);
    printf("\nArgs amount: %d\n\n", count);
}