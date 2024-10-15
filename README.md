# spDa 

A simple generic implementation of dynamic array in C. 

### Installation

1. Clone the repository:
   ```
   git clone https://github.com/SamarthPyati/spda
   ```

2. Compilation 

Build the dynamic Library
```
make build                  
```
3. Include `spDa.h` in your project and link with dynamic library by `-lspda`.

## Usage

Basic Example:

```c 
#include <stdio.h>
#include "spDa.h"

int main(void)
{      
    srand(time(NULL));  // Set random seed 
    // Elements to add in array 
    int items[10] = {3, 48, 1, 34, 0, 12, 3, 4, -1, -100};

    // Create array and print array metadata 
    int *a = spda_create(int);
    spda_print_metadata(a);
    
    spda_append_items(a, items, CARRAY_LEN(items));     // append items[] to array 
    spda_print(a, printInt);
    
    spda_append_many(a, 0, 0, 0, 0);                    // append variadic elements 
    spda_print(a, printInt);


    for (int i = 0; i < 3; ++i) spda_pop(a);            // pop last 3 elements
    spda_print(a, printInt);

    for (int i = 0; i < 3; ++i) spda_remove(a, 0);      // remove first 3 elements
    spda_print(a, printInt);

    for (int i = 0; i < 3; ++i)                         // insert 3 elements with value 1000 into 3 index
        spda_insert(a, 3, 1000);        
    spda_print(a, printInt);

    spda_clear(a);                                      // clear all elements 
    spda_print(a, printInt);

    spda_destroy(a);
    return 0;
}
```

Complex Example:

```c
#include "spDa.h"
#include <stdio.h>

typedef struct {
    double x, y, z;
} Point3D;

void printPoint3D(void *p) {
    printf("Point3D(%.1f, %.1f, %.1f)\n", 
           ((Point3D *)p)->x, ((Point3D *)p)->y, ((Point3D *)p)->z);
}

int main() {
    Point3D *points = spda_create(Point3D);
    
    Point3D p1 = {1.0, 2.0, 3.0};
    Point3D p2 = {4.0, 5.0, 6.0};
    Point3D p2 = {7.0, 8.0, 10.0};
    
    spda_append_many(points, p1, p2, p3);
    
    printf("Array contents:\n");
    spda_print(points, printPoint3D);
    
    spda_destroy(points);
    return 0;
}
```

## API Reference

### Creation and Destruction

- `spda_create(type)`: Create a new dynamic array for the specified type
- `spda_reserve(type, capacity)`: Create a new dynamic array with a specified initial capacity
- `spda_destroy(array)`: Destroy the dynamic array and free its memory

### Element Manipulation

- `spda_append(array, value)`: Append a value to the end of the array
- `spda_append_many(array, ...)`: Append all variadic number of values to the end of the array
- `spda_append_items(array, items, count)`: Append `count` number of items from array `items` to end of the array 
- `spda_pop(array)`: Remove the last element from the array
- `spda_pop_ret(array)`: Remove the last element from the array and returns the value
- `spda_insert(array, idx, value)`: Insert a value at the specified index
- `spda_remove(array, idx)`: Remove an element at the specified index
- `spda_remove_ret(array, idx, dest)`: Remove an element at the specified index and returns the value

### Information and Metadata

- `spda_len(array)`: Get the current number of elements in the array
- `spda_cap(array)`: Get the current capacity of the array
- `spda_stride(array)`: Get the size of each element in the array

### Utility Functions

- `spda_print(array, spdaElemPrinter)`: Print the contents of the array using a custom 'spdaElemPrinter' printer function

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments
- Inspired by the dynamic array implementation of [Travis Vroman`s](https://github.com/travisvroman/kohi/blob/main/kohi.core/src/containers/darray.h) game engine.

