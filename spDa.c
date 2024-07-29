#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "spDa.h"


void *_spda_create(size_t cap, size_t stride)
{
    size_t header_size = FIELD_COUNT * sizeof(size_t);
    size_t array_size = cap * stride;
    size_t *array = (size_t *) malloc(header_size + array_size);
    if (!array)
    {
        raise("MEM_ALLOCATION", "Failed memory allocation for dynamic array.");
        exit(EXIT_FAILURE);
    }
    array[CAPACITY] = cap;
    array[LENGTH] = 0;
    array[STRIDE] = stride;
    return (void *)(array + FIELD_COUNT);
}

void _spda_destroy(void *array)
{   
    size_t* header = (size_t *)array - FIELD_COUNT;
    free(header);
}

size_t _spda_field_get(void *array, size_t field)
{
    size_t* header = (size_t *)array - FIELD_COUNT;
    return header[field];
}

void _spda_field_set(void *array, size_t field, size_t value)
{
    size_t* header = (size_t *)array - FIELD_COUNT;
    header[field] = value;
}

void *_spda_resize(void *array)
{   
    size_t *header = (size_t *)array - FIELD_COUNT;
    size_t new_cap = header[CAPACITY] * SPDA_GROWTH_FACTOR;         // cap * 2
    size_t new_size = (FIELD_COUNT * sizeof(size_t)) + (new_cap * header[STRIDE]);
    if (header == NULL) 
    {
        raise("MEM_ALLOCATION", "Header was not allocated properly."); 
        exit(EXIT_FAILURE);
    }
    header = realloc(header, new_size);
    if (header == NULL)
    {
        raise("MEM_ALLOCATION","Failed to realloc the header and the array.");
        exit(EXIT_FAILURE);
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
        void *new_array = _spda_resize(array);
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

void *_spda_resize_spec(void *array, size_t size)
{
    size_t *header = (size_t *)array - FIELD_COUNT;
    size_t new_cap = size;         
    size_t new_size = (FIELD_COUNT * sizeof(size_t)) + (new_cap * header[STRIDE]);
    if (header == NULL) 
    {
        raise("MEM_ALLOCATION", "Header was not allocated properly."); 
        exit(EXIT_FAILURE);
    }
    header = realloc(header, new_size);
    if (header == NULL)
    {
        raise("MEM_ALLOCATION","Failed to realloc the header and the array.");
        exit(EXIT_FAILURE);
    }
    header[CAPACITY] = new_cap;
    return (void *)(header + FIELD_COUNT);
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
        raise("INDEX_OUT_OF_BOUNDS", "Cannot pop elements from an empty array."); 
        return;
    }
    _spda_field_set(array, LENGTH, length - 1);
}

void _spda_pop_ret(void *array, void *dest) 
{
    size_t length = spda_len(array);
    size_t stride = spda_stride(array);
    if (length == 0) {
        raise("INDEX_OUT_OF_BOUNDS","Cannot pop elements from an empty array."); 
        return;
    }
    memcpy(dest, (char *)array + length * stride, stride);
    _spda_field_set(array, LENGTH, length - 1);
}


void *_spda_insert(void *array, int idx, const void* value)
{
    size_t length = spda_len(array);
    size_t stride = spda_stride(array);
    size_t capacity = spda_cap(array);
    if (idx < 0 || (size_t)idx > length) {
        raise("INDEX_OUT_OF_BOUNDS","Index out of bounds for insert.");
        return array;
    }
    if (length >= capacity)
    {
        array = _spda_resize(array);
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
        raise("INDEX_OUT_OF_BOUNDS","Index out of bounds for remove.");
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
        raise("INDEX_OUT_OF_BOUNDS", "Index out of bounds for remove.");
        return array;
    }
    memcpy(dest, (char *)array + idx * stride, stride);
    memmove((char *)array + idx * stride, (char *)array + (idx + 1) * stride, (length - idx - 1) * stride);
    _spda_field_set(array, LENGTH, length - 1);
    return array;
}

void spda_print(void *array, void (*spdaElemPrinter)(void *elem))
{   
    for (size_t i = 0; i < spda_len(array); ++i) 
    {   
        void *p = (array + i * spda_stride(array));
        spdaElemPrinter(p);
    }
}
