#include "../spDa.h"

int compar(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

int main(void)
{
    printf("Testing copy and sorting ..\n");
    int *a = spda_create(int);
    spda_append_many(a, 2, 3, 10, 18, 19, 0, 1);

    printf("Original array: ");
    spda_print(a, printInt);

    printf("Copied array: ");
    void *cpy = spda_copy(a);
    spda_print(cpy, printInt);
    
    printf("Sorted array: ");
    spda_sort(a, compar);
    spda_print(a, printInt);
    
    // spda_destroy(cpy);
    spda_destroy(a);

    // String arrays
    printf("\n\nTesting string array ..\n");
    const char **sa = spda_create(const char *);
    spda_append_many(sa, "Apple", "Banana", "Cherry", "Dark Chocolate", "Eli Lilly");
    spda_pop(sa);
    spda_remove(sa, 1);
    spda_print_metadata(sa);
    spda_print(sa, printStr);
    spda_destroy(sa);

    return 0;
}