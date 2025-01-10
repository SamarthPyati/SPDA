#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../spda.h"


// Helper function to compare doubles with a small epsilon for floating-point precision
int double_equals(double a, double b) {
    return fabs(a - b) < 1e-9;
}
int comparInt(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

int main() {
    // Test creation
    double *array = spda_create(double);
    assert(spda_len(array) == 0);
    assert(spda_cap(array) == SPDA_DEFAULT_CAPACITY);
    assert(spda_stride(array) == sizeof(double));

    // Test append
    for (int i = 0; i < 10; i++) {
        double value = i + 0.5;
        spda_append(array, value);
    }
    assert(spda_len(array) == 10);
    assert(spda_cap(array) >= 10);

    // Test print
    printf("Array contents: ");
    spda_print(array, printDouble);
    printf("\n");   

    // Test insert
    spda_insert(array, 5, 100.75);
    assert(spda_len(array) == 11);
    assert(double_equals(array[5], 100.75));

    // Test remove
    double removed;
    spda_remove_ret(array, 5, &removed);
    assert(spda_len(array) == 10);
    assert(double_equals(removed, 100.75));

    // Test pop
    double popped;
    _spda_pop_ret(array, &popped);
    assert(spda_len(array) == 9);
    assert(double_equals(popped, 9.5));

    // Test clear
    spda_clear(array);
    assert(spda_len(array) == 0);

    // Test resize
    size_t old_cap = spda_cap(array);
    array = _spda_resize_def(array);
    assert(spda_cap(array) == old_cap * SPDA_GROWTH_FACTOR);


    // Test destruction
    spda_destroy(array);

    // Sorting the array 
    int *a = spda_create(int);
    spda_append_many(a, 1, 31, 57, 1, 9, 0, 12, 4, -1, -10);
    printf("Before: ");
    spda_print(a, printInt);
    spda_sort(a, comparInt);
    spda_print(a, printInt);


    printf("\nAll tests passed successfully!\n");
 
    return 0;
}