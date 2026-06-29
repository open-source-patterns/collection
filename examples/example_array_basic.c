/**
 * @file example_array_basic.c
 * @brief Basic Array Example
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include "collection/i_array.h"

#include <stdio.h>

void array_usage1();
void array_usage2();

int main() {
    array_usage1();
    array_usage2();

    return 0;
}

void array_usage1() {
    struct IArray *array = collection_array_new();

    const char *item1 = "Hello";
    const char *item2 = "World";

    array->push(array, item1);
    array->unshift(array, item2);

    printf("Array size: %zu\n", array->count(array));

    collection_array_dealloc(&array, NULL);
}

void array_usage2() {
    struct IArray *array = collection_array_new();

    array->push(array, "Apple");
    array->push(array, "Banana");
    array->push(array, "Orange");

    while (array->count(array) > 0) {
        printf("Fruit: %s\n", (const char *) array->shift(array)); // Apple Banana Orange
    }

    collection_array_dealloc(&array, NULL);
}