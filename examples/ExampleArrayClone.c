#include <stdio.h>
#include "collection/IArray.h"

void print_and_free(void *item) {
    printf("Freeing item: %s\n", (char *)item);
    // Note: this example assumes items do not need actual free().
}

int main() {
    struct IArray *original = collection_array_new();
    original->push(original, "One");
    original->push(original, "Two");
    original->push(original, "Three");

    struct IArray *clone = original->clone(original);
    printf("Clone size: %d\n", clone->size(clone));

    original->clear(original, print_and_free);

    printf("Original size after clear: %d\n", original->size(original));
    printf("Clone size remains: %d\n", clone->size(clone));

    collection_array_free(&original);
    collection_array_free(&clone);

    return 0;
}
