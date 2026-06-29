/**
 * @file example_array_clone.c
 * @brief Array Clone Example
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include "collection/i_array.h"

#include <stdio.h>

int main() {
    struct IArray *array = collection_array_new();

    array->push(array, "One");
    array->push(array, "Two");
    array->push(array, "Three");

    struct IArray *clone = array->clone(array);
    printf("Clone size: %zu\n", clone->count(clone));

    collection_array_dealloc(&array, NULL);
    collection_array_dealloc(&clone, NULL);

    return 0;
}
