/**
* @file ExampleArrayClone.c
 * @brief Array Clone Example
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include <stdio.h>
#include "collection/IArray.h"

void print_and_free(void *item) {
    printf("Freeing item: %s\n", (char *)item);
    // Note: this example assumes items do not need actual free().
}

int main() {
    const char *error = NULL;
    struct IArray *original = collection_array_new(&error);
    original->push(original, "One", &error);
    original->push(original, "Two", &error);
    original->push(original, "Three", &error);

    struct IArray *clone = original->clone(original, &error);
    printf("Clone size: %d\n", clone->size(clone));

    original->clear(original, print_and_free);

    printf("Original size after clear: %d\n", original->size(original));
    printf("Clone size remains: %d\n", clone->size(clone));

    collection_array_free(&original);
    collection_array_free(&clone);

    return 0;
}
