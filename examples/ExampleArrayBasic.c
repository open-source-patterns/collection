#include <assert.h>
#include <stdio.h>

#include "collection/IArray.h"

int main() {
    struct IArray *array = collection_array_new();
    if (!array) {
        fprintf(stderr, "Failed to create array\n");
        return 1;
    }

    const char *item1 = "Hello";
    const char *item2 = "World";

    array->push(array, item1);
    array->unshift(array, item2);

    printf("Array size: %d\n", array->size(array));

    collection_array_free(&array);
    return 0;
}
