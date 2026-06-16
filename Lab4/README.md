# The fourth lab.

**HAS:**

## Task 1 – swap function
Implement `void swap(int *a, int *b)` that swaps the values of two integers using pointers.

**Bonus:** write at least 10 comprehensive tests (including edge cases: `INT_MAX` / `INT_MIN`, identical values, same pointer, zeros, etc.) using the **MinUnit** testing framework.

## Task 2 – Finding minimum in 2D arrays
Implement two versions:

- `myMinStatic` — for static (fixed-size) 2D arrays `int arr[N][M]`
- `myMinDynamic` — for fully dynamic 2D arrays `int **arr` (both dimensions passed as parameters)

## Task 3 – Custom string comparison
Implement `int myStrCmp(const char *a, const char *b)` that behaves **exactly** like standard `strcmp`:

- returns negative if `a < b`,
- zero if equal,
- positive if `a > b`

(lexicographic order, null-terminated strings).

## Task 4 – Dynamic array without duplicates
Create a dynamic integer array, fill it with random values.  
Write a function that inserts a new value **only if it doesn’t already exist** (resize array with `realloc` when needed).  
Include a function to print the entire array.

## Task 5 – Recursive functions
- `5a.` `sumRecursive(int n)` — sum of the first **n** natural numbers
- `5b.` `fibRecursive(int n)` — the **n**-th Fibonacci number

## Task 6 – Functions with variable number of arguments
- `6a.` Implement `varArgs` **without** using `va_list` macros. List ends with a zero argument.
- `6b.` Reimplement the same function **using** the standard `va_list`, `va_start`, `va_arg`, `va_end`.

## Task 7 – Function returning a pointer to the minimum
Implement `int* myMin(int *arr, int size)` that returns a pointer to the smallest element in the array.

**Requirement:** the call must be usable on the **left side** of an assignment.

The program runs all MinUnit tests and demonstrates every implemented function.
