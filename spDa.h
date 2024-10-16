/*
**  @brief: A Generic Dynamic Array Implementation in C **
*   @Author: Samarth Pyati 
*   @Date : 15-10-2024
*   @Version : 1.4
*/

#ifndef SPDA_H_
#define SPDA_H_

#include <stdio.h>          // size_t 

/* 
** Memory Layout -
* size_t capacity = max elements that array can contain;
* size_t length = number of elements the array actually contains;
* size_t stride = size of each items in array;
* void *elements;
*/

typedef enum {
    CAPACITY,               // capacity 
    LENGTH,                 // length 
    STRIDE,                 // stride
    FIELD_COUNT             // number of fields
} SPDA_FIELD;

#define SPDA_DEF_CAPACITY 1        
#define SPDA_GROWTH_FACTOR 2

#define raise(etype, msg)                                              \
    do {                                                               \
        fprintf(stderr, "%s:%s\n", etype, msg);                        \
    } while (0)                                                        

// Length for vanilla c arrays
#define CARRAY_LEN(xs) (sizeof((xs)) / sizeof((xs)[0]))

void *_spda_create(size_t cap, size_t stride);
void _spda_destroy(void *array);

size_t _spda_field_get(void *array, size_t field);
void _spda_field_set(void *array, size_t field, size_t value);

void *_spda_resize(void *array);
void *_spda_resize_spec(void *array, size_t size);      // resize the array by a specific amount 

void *_spda_append(void *array, const void* value);
void *_spda_append_many(void *array, void *items, size_t item_count);
void _spda_pop(void *array);

void *_spda_insert(void *array, int idx, const void* value);
void *_spda_remove(void *array, int idx);

void _spda_pop_ret(void *array, void *dest);                 // return the popped item
void *_spda_remove_ret(void *array, int idx, void *dest);    // return the popped item


// Utilities 
void *spda_copy(void *src);
void *spda_search(void *array, const void *target);                       
void spda_sort(void *array, int (*compar)(const void *, const void *));   // qsort
void spda_print_metadata(void *array);
void spda_print(void *array, void (*spdaElemPrinter)(void *elem));
void spda_clear(void *array);

// Random Helper Functions
int get_rand(int min, int max);
float get_randf(float min, float max);

// Random Array 
void spda_rand(int **array, size_t n, int min, int max);                 // Populate Random int array 
void spda_randf(float **array, size_t n, float min, float max);          // Populate Random double array 

// Default Printer Functions
void printInt(void* elem);
void printFloat(void *elem);
void printDouble(void *elem);
void printChar(void *elem);
void printStr(void *elem);

// MACROS 
#define spda_create(type) \
    (type *) _spda_create(SPDA_DEF_CAPACITY, sizeof(type))

#define spda_reserve(type, capacity) \
    (type *) _spda_create(capacity, sizeof(type))    

#define spda_destroy(array)  _spda_destroy(array)

#define spda_append(array, value)                    \
    do {                                             \
        __auto_type temp = (value);                  \
        (array) = _spda_append((array), &temp);      \
    } while (0)

#define spda_append_many(array, ...)                                        \
    do {                                                                    \
        __typeof__(*(array)) _temp[] = {__VA_ARGS__};                       \
        (array) = _spda_append_many((array), _temp, CARRAY_LEN(_temp));     \
    } while (0)

#define spda_append_items(array, items, count)                            \
    do {                                                                  \
            (array) = _spda_append_many((array), (items), (count));       \
    } while (0);                                                          

#define spda_pop(array) _spda_pop(array)
#define spda_pop_ret(array, dest) _spda_pop_ret(array, dest)

#define spda_insert(array, idx, value)                      \
    do {                                                    \
        __auto_type temp = (value);                         \
        (array) = _spda_insert((array), (idx), &temp);      \
    } while (0)

#define spda_remove(array, idx) _spda_remove((array), idx)
#define spda_remove_ret(array, idx, dest) _spda_remove_ret((array), idx, dest)

#define spda_clear(array) \
    _spda_field_set((array), LENGTH, 0)

#define spda_set_length(array, value) \
    _spda_field_set((array), LENGTH, value)

#define spda_len(array) \
    _spda_field_get((array), LENGTH)

#define spda_cap(array) \
    _spda_field_get((array), CAPACITY)

#define spda_stride(array) \
    _spda_field_get((array), STRIDE)

#endif // SPDA_H_
