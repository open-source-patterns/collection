/**
* @file ExampleArrayRemove.c
 * @brief Array Remove Item Example
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include <stdio.h>
#include "collection/IArray.h"

int main() {
    const char *error = NULL;
    struct IArray *array = collection_array_new(&error);
    array->push(array, "Apple", &error);
    array->push(array, "Banana", &error);
    array->push(array, "Cherry", &error);

    printf("Contains 'Banana'? %s\n", array->containsValue(array, "Banana") ? "Yes" : "No");

    const void *removed = array->removeItem(array, "Banana");
    printf("Removed item: %s\n", (const char *)removed);

    printf("Contains 'Banana'? %s\n", array->containsValue(array, "Banana") ? "Yes" : "No");

    collection_array_free(&array);
    return 0;
}
