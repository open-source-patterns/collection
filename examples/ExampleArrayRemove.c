#include <stdio.h>
#include "collection/IArray.h"

int main() {
    struct IArray *array = collection_array_new();
    array->push(array, "Apple");
    array->push(array, "Banana");
    array->push(array, "Cherry");

    printf("Contains 'Banana'? %s\n", array->containsValue(array, "Banana") ? "Yes" : "No");

    const void *removed = array->removeItem(array, "Banana");
    printf("Removed item: %s\n", (const char *)removed);

    printf("Contains 'Banana'? %s\n", array->containsValue(array, "Banana") ? "Yes" : "No");

    collection_array_free(&array);
    return 0;
}
