/*
**  @brief: A Generic Dynamic Array Implementation in C **
*   @Author: Samarth Pyati 
*   @Date : 10-01-2025
*   @Version : 1.7
*/

#ifndef SPDA_H_
#define SPDA_H_

#include <stdio.h>          // size_t 
#include <stdbool.h>        // bool

/* 
** Memory Layout **
* size_t capacity = max elements that array can contain;
* size_t length = number of elements the array actually contains;
* size_t stride = size of each items in array;
* void *elements = elements in array;
*
*  Structure of the array: 
*                 METADATA                                 ELEMENTS
*  +---------------------------------------+---------------------------------------+
*  |  capacity   |  length   |  stride     |               elements                |
*  +---------------------------------------+---------------------------------------+
*  |  (size_t)   |  (size_t) |  (size_t)   |               (void *)                |
*  +---------------------------------------+---------------------------------------+
*                                          ^
*                                      array pointer 
*/

typedef enum {
    CAPACITY,               // capacity 
    LENGTH,                 // length 
    STRIDE,                 // stride
    FIELD_COUNT             // number of fields
} SPDA_FIELD;

#define SPDA_DEFAULT_CAPACITY 8
#define SPDA_GROWTH_FACTOR 2    
#define SPDA_SHRINK_THRESHOLD 0.25 // if array utilisation falls below 25% shrink the capacity of array 

/* ERROR HANDLING */
#define raise(etype, msg)                                                               \
    do {                                                                                \
        fprintf(stderr, "%s:%d [%s] - %s\n", __FILE__, __LINE__, etype, msg);           \
    } while (0)                                                         

// Length for vanilla c arrays
#define CARRAY_LEN(xs) (sizeof((xs)) / sizeof((xs)[0]))

/* Core Operations */
void *_spda_create(size_t cap, size_t stride);
void _spda_destroy(void *array);
bool _spda_is_valid(const void *array);

/* Field Operations */
size_t _spda_field_get(void *array, size_t field);
void _spda_field_set(void *array, size_t field, size_t value);

/* Memory Operations */
void *_spda_resize_def(void *array);
void *_spda_resize(void *array, size_t size);     
void *spda_shrink(void *array);

/* Array Operations */
void *_spda_append(void *array, const void* value);
void *_spda_append_many(void *array, void *items, size_t item_count);
void *_spda_insert(void *array, int idx, const void* value);

void _spda_pop(void *array);
void _spda_pop_ret(void *array, void *dest);                 // return the popped item

void *_spda_remove(void *array, int idx);
void *_spda_remove_ret(void *array, int idx, void *dest);    // return the removed item

void _spda_reverse(void *array);                             // reverse array inplace

/* Sort and search */
void spda_sort(void *array, int (*compar)(const void *, const void *));   // qsort  
void *spda_search(void *array, const void *target); // TODO                      

/* Utilities */ 
void *spda_copy(void *src);
void spda_clear(void *array);
void spda_print(void *array, void (*spdaElemPrinter)(void *elem));
void spda_print_metadata(void *array);

/* Random Helper Functions */ 
int get_rand(int min, int max);
float get_randf(float min, float max);

/* Random Array Generation */
void spda_rand(int **array, size_t n, int min, int max);                 // Populate Random int array 
void spda_randf(float **array, size_t n, float min, float max);          // Populate Random double array 

/* Default datatype element printer */
void _printInt(void *elem);
void _printFloat(void *elem);
void _printDouble(void *elem);
void _printChar(void *elem);
void _printStr(void *elem);

#define printInt _printInt
#define printFloat _printFloat
#define printDouble _printDouble
#define printChar _printChar
#define printStr _printStr

/* Macros */
#define spda_create(type) \
    (type *) _spda_create(SPDA_DEFAULT_CAPACITY, sizeof(type))

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

#define spda_foreach(type, array, varname) \
    for (size_t _spda_idx = 0; \
         _spda_is_valid(array) && _spda_idx < spda_len(array); \
         ++_spda_idx) \
        for (type varname = (array)[_spda_idx], *_spda_flag = (type*)1; \
             _spda_flag; \
             _spda_flag = NULL)

#define spda_insert(array, idx, value)                      \
    do {                                                    \
        __auto_type temp = (value);                         \
        (array) = _spda_insert((array), (idx), &temp);      \
    } while (0)

#define spda_remove(array, idx) _spda_remove((array), idx)
#define spda_remove_ret(array, idx, dest) _spda_remove_ret((array), idx, dest)

#define spda_reverse(array) \
    _spda_reverse((array))

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
