/**
* @file ExampleArrayBasic.c
 * @brief Basic Array Example
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include <assert.h>
#include <stdio.h>

#include "collection/IArray.h"

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
    return 0;
}
