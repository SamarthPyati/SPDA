#include "../spDa.h"

void printInt(void *elem)
{
    printf("%d ", *(int *)elem);
}

int compar(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

int main(void)
{
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

    return 0;
}