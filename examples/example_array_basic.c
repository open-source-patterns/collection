/**
 * @file example_array_basic.c
 * @brief Basic Array Example
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include "collection/i_array.h"

#include <stdio.h>

static void array_usage1(void);
static void array_usage2(void);

int main(void) {
    array_usage1();
    array_usage2();

    return 0;
}

static void array_usage1(void) {
    struct IArray *array = collection_array_new(); // Create an array.
    if (array == NULL) return;

    // Insert elements.
    array->push(array, "Hello");
    array->unshift(array, "World");

    printf("Array size: %zu\n", array->count(array)); // Display the number of elements.

    collection_array_dealloc(&array, NULL); // Clean up.
}

static void array_usage2(void) {
    struct IArray *array = collection_array_new(); // Create an array.
    if (array == NULL) return;

    array->push(array, "Apple");
    array->push(array, "Banana");
    array->push(array, "Orange");

    while (array->count(array) > 0) {
        printf("Fruit: %s ", (const char *) array->shift(array)); // Prints: Apple Banana Orange
    }

    collection_array_dealloc(&array, NULL); // Clean up.
}