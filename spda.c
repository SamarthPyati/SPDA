#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "spda.h"

bool _spda_is_valid(const void *array) {
    if (!array) return false;
    size_t *header = (size_t *)array - FIELD_COUNT;
    return header && header[STRIDE] > 0;
}

void *_spda_create(size_t cap, size_t stride)
{   
    if (cap < SPDA_DEFAULT_CAPACITY) cap = SPDA_DEFAULT_CAPACITY;
    if (stride == 0) {
        raise("INVALID_ARGUMENT", "Stride (size of datatype) cannot be zero");
        return NULL;
    }
    size_t header_size = FIELD_COUNT * sizeof(size_t);
    size_t array_size = cap * stride;
    size_t *array = (size_t *) malloc(header_size + array_size);

    if (!array) {
        raise("MEM_ALLOCATION", "Failed memory allocation for dynamic array.");
        return NULL;
    }

    array[CAPACITY] = cap;
    array[LENGTH] = 0;
    array[STRIDE] = stride;
    return (void *)((size_t *)array + FIELD_COUNT);
}

void _spda_destroy(void *array)
{   
    if (!_spda_is_valid(array)) return;
    size_t* header = (size_t *)array - FIELD_COUNT;
    free(header);
}

size_t _spda_field_get(void *array, size_t field)
{
    if (!_spda_is_valid(array)) return -1;
    size_t* header = (size_t *)array - FIELD_COUNT;
    return header[field];
}

void _spda_field_set(void *array, size_t field, size_t value)
{
    if (!_spda_is_valid(array)) return;
    size_t* header = (size_t *)array - FIELD_COUNT;
    header[field] = value;
}

void *_spda_resize_def(void *array)
{   
    if (!_spda_is_valid(array)) return NULL;

    size_t *header = (size_t *)array - FIELD_COUNT;
    size_t new_cap = (size_t) header[CAPACITY] * SPDA_GROWTH_FACTOR;      
    size_t new_size = (FIELD_COUNT * sizeof(size_t)) + (new_cap * header[STRIDE]);
    if (header == NULL) 
    {
        raise("MEM_ALLOCATION", "Array header was not allocated properly."); 
        return NULL;
    }

    header = realloc(header, new_size);
    
    if (header == NULL)
    {
        raise("MEM_ALLOCATION", "Failed to reallocate the header and the array.");
        return NULL;
    }
    header[CAPACITY] = new_cap;
    return (void *)((size_t *)header + FIELD_COUNT);
}

void *_spda_resize(void *array, size_t size)
{
    if (!_spda_is_valid(array)) return NULL;

    size_t *header = (size_t *)array - FIELD_COUNT;
    size_t new_cap = size;         
    size_t new_size = (FIELD_COUNT * sizeof(size_t)) + (new_cap * header[STRIDE]);
    if (header == NULL) 
    {
        raise("MEM_ALLOCATION", "Array header was not allocated properly."); 
        return NULL;
    }

    header = realloc(header, new_size);

    if (header == NULL)
    {
        raise("MEM_ALLOCATION", "Failed to reallocate the header and the array.");
        return NULL;
    }

    header[CAPACITY] = new_cap;
    return (void *)(header + FIELD_COUNT);
}

void *_spda_append(void *array, const void* value)
{   
    size_t length = spda_len(array);
    size_t stride = spda_stride(array);
    size_t capacity = spda_cap(array);
    if (length >= capacity)
    {
        void *new_array = _spda_resize_def(array);
        if (new_array == NULL) {
            raise("MEM_ALLOCATION", "Failed to resize array");
            return array;  // Return original array if resize fails
        }
        array = new_array;  
    }

    memcpy((char*)array + length * stride, value, stride);
    _spda_field_set(array, LENGTH, length + 1);     // increment length
    return array;
}

void *_spda_append_many(void *array, void *items, size_t item_count)
{
    size_t stride = spda_stride(array);
    char *item_ptr = (char *)items;
    for (size_t i = 0; i < item_count; ++i)
    {
        array = _spda_append(array, item_ptr);
        item_ptr += stride;
    }
    return array;
}

void _spda_pop(void *array)
{   
    size_t length = spda_len(array);
    if (length == 0) {
        raise("INDEX_OUT_OF_BOUNDS", "Cannot pop elements from an empty array"); 
        return;
    }
    _spda_field_set(array, LENGTH, length - 1);
}

void _spda_pop_ret(void *array, void *dest) 
{
    size_t length = spda_len(array);
    size_t stride = spda_stride(array);
    if (length == 0) {
        raise("INDEX_OUT_OF_BOUNDS","Cannot pop elements from an empty array"); 
        return;
    }
    memcpy(dest, (char *)array + ((length - 1) * stride), stride);
    _spda_field_set(array, LENGTH, length - 1);
}

void *_spda_insert(void *array, int idx, const void* value)
{
    size_t length = spda_len(array);
    size_t stride = spda_stride(array);
    size_t capacity = spda_cap(array);
    if (idx < 0 || (size_t)idx > length) {
        raise("INDEX_OUT_OF_BOUNDS", "Index out of bounds for insert");
        return array;
    }
    if (length >= capacity)
    {
        array = _spda_resize_def(array);
    }
    memmove((char *)array + (idx + 1) * stride, (char *)array + idx * stride, (length - idx) * stride);
    memcpy((char *)array + idx * stride, value, stride);
    _spda_field_set(array, LENGTH, length + 1);
    return array;
}

void *_spda_remove(void *array, int idx)
{
    size_t length = spda_len(array);
    size_t stride = spda_stride(array);
    if (idx < 0 || (size_t)idx >= length) {
        raise("INDEX_OUT_OF_BOUNDS", "Index out of bounds for remove");
        return array;
    }
    memmove((char *)array + idx * stride, (char *)array + (idx + 1) * stride, (length - idx - 1) * stride);
    _spda_field_set(array, LENGTH, length - 1);
    return array;
}

void *_spda_remove_ret(void *array, int idx, void *dest)
{
    size_t length = spda_len(array);
    size_t stride = spda_stride(array);
    if (idx < 0 || (size_t)idx >= length) {
        raise("INDEX_OUT_OF_BOUNDS", "Index out of bounds for remove");
        return array;
    }
    memcpy(dest, (char *)array + idx * stride, stride);
    memmove((char *)array + idx * stride, (char *)array + (idx + 1) * stride, (length - idx - 1) * stride);
    _spda_field_set(array, LENGTH, length - 1);
    return array;
}

void _spda_reverse(void *array) {
    if (!_spda_is_valid(array)) return;
    
    size_t stride = spda_stride(array);
    size_t len = spda_len(array);
    char *temp = malloc(stride);

    if (!temp) {
        raise("MEM_ALLOCATION", "Failed to allocate temporary buffer");
        return;
    }

    for (size_t i = 0; i < len / 2; ++i) {
        char *a = (char *)array + i * stride;
        char *b = (char *)array + ((len - i - 1) * stride);   
        memcpy(temp, a, stride);
        memcpy(a, b, stride);
        memcpy(b, temp, stride);
    }
    free(temp);
}

void *spda_copy(void *src)
{
    if (src == NULL) {
        raise("SPDA_COPY_ERR", "source array is NULL");
        return NULL;
    }

    size_t capacity = spda_cap(src);
    size_t length = spda_len(src);
    size_t stride = spda_stride(src);
    size_t arr_size = capacity * stride;
    size_t header_size = sizeof(size_t) * FIELD_COUNT;
    
    void *_dst = _spda_create(capacity, stride);
    if (_dst == NULL)
    {
        raise("SPDA_COPY", "Failed to allocate memory for the new array.");
        return NULL;
    }
    memcpy(_dst, (size_t *)src - FIELD_COUNT, arr_size + header_size);
    _spda_field_set(_dst, LENGTH, length);
    return (void *)((size_t *)_dst + FIELD_COUNT);
}

void spda_sort(void *array, int (*compar)(const void *, const void *))
{   
    if (!array) {
        raise("SPDA_SORTING", "Array is NULL!");
        exit(EXIT_FAILURE);
    }

    size_t length = spda_len(array);
    size_t stride = spda_stride(array);

    qsort(array, length, stride, compar);
}

void spda_print_metadata(void *array)
{   
    if (!array)
    {
        raise("SPDA_UNINITIALISED", "array not initialised properly!");
        exit(EXIT_FAILURE);
    }
    /* Printing the Metadata of the array */
    printf("Capacity: %zu, Length: %zu, Stride (bits): %zu\n", 
        spda_cap(array), spda_len(array), spda_stride(array) * 8);
}

void spda_print(void *array, void (*spdaElemPrinter)(void *elem))
{   
    for (size_t i = 0; i < spda_len(array); ++i) 
    {   
        void *p = (array + i * spda_stride(array));
        spdaElemPrinter(p);
    }
    printf("\n");
}

/* Default `spdaElemPrinter` functions for convenience */
void _printInt(void *elem)
{
    printf("%d ", *(int *)elem);
}

void _printFloat(void *elem)
{
    printf("%f ", *(float *)elem);
}

void _printDouble(void *elem)
{
    printf("%lf ", *(double *)elem);
}

void _printChar(void *elem)
{
    printf("%c ", *(char *)elem);
}

void _printStr(void *elem)
{
    printf("%s\n", *(const char **)elem);
}

/* Random Function Utilities */
int get_rand(int min, int max) 
{
    return rand() % (max - min + 1) + min;
}

float get_randf(float min, float max) 
{
    float f;
    f = ((float)rand() / ((float)RAND_MAX + 1));
    return (min + f * (max - min));
}

/* Generating random arrays */
void spda_rand(int **array, size_t n, int min, int max)
{   
    for (size_t i = 0; i < n; ++i)
    {   
        int random_value = get_rand(min, max);
        *array = _spda_append(*array, &random_value);
    }
}

void spda_randf(float **array, size_t n, float min, float max)
{   
    for (size_t i = 0; i < n; ++i)  
    {   
        float random_value = get_randf(min, max);
        *array = _spda_append(*array, &random_value);
    }
}
