/**
* @file ExampleArrayBasic.c
 * @brief Basic Array Example
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include <stdio.h>

#include "collection/IArray.h"

void array_usage();

int main() {
    const char *error = NULL;
    struct IArray *array = collection_array_new(&error);
    if (!array) {
        fprintf(stderr, "Failed to create array\n");
        return 1;
    }

    const char *item1 = "Hello";
    const char *item2 = "World";

    array->push(array, item1, &error);
    array->unshift(array, item2, &error);

    printf("Array size: %d\n", array->size(array));

    collection_array_free(&array);

    array_usage();

    return 0;
}

void array_usage() {
    const char *error = NULL;
    struct IArray *array = collection_array_new(&error);
    array->push(array, "Apple", &error);
    array->push(array, "Banana", &error);
    array->push(array, "Orange", &error);

    while (array->size(array) > 0) {
        printf("Fruit: %s\n", (const char *)array->shift(array)); // Apple Banana Orange
    }

    collection_array_free(&array);
}