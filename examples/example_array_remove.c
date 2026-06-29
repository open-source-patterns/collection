/**
 * @file example_array_remove.c
 * @brief Array Remove Item Example
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include "collection/i_array.h"

#include <stdio.h>

int main() {
    struct IArray *array = collection_array_new();

    array->push(array, "Apple");
    array->push(array, "Banana");
    array->push(array, "Cherry");

    printf("Contains 'Banana'? %s\n", array->contains_value(array, "Banana") ? "Yes" : "No");

    const void *removed = array->remove_item(array, "Banana");
    printf("Removed item: %s\n", (const char *)removed);

    printf("Contains 'Banana'? %s\n", array->contains_value(array, "Banana") ? "Yes" : "No");

    collection_array_dealloc(&array, NULL);

    return 0;
}
