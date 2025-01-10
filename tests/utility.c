#include "../spda.h"

int compar(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void selection_sort(void);
void insertion_sort(void);
void merge_sort(void);

int main(void)
{   
    int *a = spda_create(int);
    spda_destroy(a);
    return 0;
}

void merge_sort(void)
{
    int *a = spda_create(int);
    spda_append_many(a, 1, 2, 34, 5, 67, 8, 3, 1, 32 ,19, 0);
    spda_print(a, printInt);
    spda_destroy(a);
}

void binary_search(void)
{
    int *a = spda_create(int);
    spda_append_many(a, 1, 35, 15, 7, 8, 9, 10, 12, 34, 4, 5, 6);
    spda_sort(a, compar);
    spda_print(a, printInt);

    int target = 12;
    int low = 0;
    int high = spda_len(a);
    int result;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        if (a[mid] == target)
        {
            result = mid;
            break;
        }
        else if (a[mid] > target) high = mid - 1;
        else if (a[mid] < target) low = mid + 1;
        else result = -1;
    }

    printf("Result at index: %d\n", result);
    spda_destroy(a);
}

void bubble_sort(void)
{
    int *a = spda_create(int);
    spda_append_many(a, 1, 35, 15, 7, 8, 9, 10, 12, 34, 4, 5, 6);
    spda_print(a, printInt);

    int swapped = -1;
    for (size_t i = 0; i < spda_len(a) - 1; ++i)
    {
        swapped = -1;
        for (size_t j = 0; j < spda_len(a) - i - 1; ++j)
        {
            if (a[j] > a[j + 1])
            {
                swap(&a[j], &a[j + 1]);
                swapped = 1;
            }
        }

        if (swapped == -1) break;
    }

    spda_print(a, printInt);
    spda_destroy(a);
}


void selection_sort(void)
{
    int *a = spda_create(int);
    spda_append_many(a, 1, 35, 15, 7, 8, 9, 10);
    printf("\t-- Selection Sort --\t\n");
    printf("Unsorted: ");
    spda_print(a, printInt);

    // selection sort
    for (size_t i = 0; i < spda_len(a) - 1; ++i)
    {
        size_t minIdx = i;
        // Select the smallest element from the unsorted part and swap it with the current key
        for (size_t j = i + 1; j < spda_len(a); ++j)
        {
            if (a[j] < a[minIdx])   minIdx = j;
        }
        // swap
        // a[i] -> current key
        // a[minIdx] -> most minm element in the unsorted part
        swap(&a[i], &a[minIdx]);
    }

    printf("Sorted  : ");
    spda_print(a, printInt);
    spda_destroy(a);
}

void insertion_sort(void)
{
    int *a = spda_create(int);
    spda_append_many(a, 1, 35, 15, 7, 8, 9, 10);
    printf("\t-- INSERTION SORT --\t\n");
    printf("Unsorted: ");
    spda_print(a, printInt);

    // insertion sort
    for (size_t i = 1; i < spda_len(a); ++i) {
        // key = a[i]
        // Check if the previous elements are greater, if greater then swap them with the key
        for (size_t j = 0; j < i; ++j)
        {
            if (a[j] > a[i])
            {
                swap(&a[j], &a[i]);
            }
        }
    }

    printf("Sorted  : ");
    spda_print(a, printInt);
    spda_destroy(a);
}

void copy_and_sort(void)
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
    
    spda_destroy(cpy);
    spda_destroy(a);
}
