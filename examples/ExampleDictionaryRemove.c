/**
* @file ExampleDictionaryRemove.c
 * @brief Dictionary Remove Item Example
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include <stdio.h>
#include "collection/IDictionary.h"

void cleanup(void *value) {
    printf("Freeing value: %s\n", (char *)value);
    // Only free if dynamically allocated
}

int main() {
    const char *error = NULL;
    struct IDictionary *dict = collection_dictionary_new(&error);
    if (error != NULL) return fprintf(stderr, "%s\n", error), 1;

    dict->put(dict, "one", "1", &error);
    if (error != NULL) return fprintf(stderr, "%s\n", error), 1;
    dict->put(dict, "two", "2", &error);
    if (error != NULL) return fprintf(stderr, "%s\n", error), 1;
    dict->put(dict, "three", "3", &error);
    if (error != NULL) return fprintf(stderr, "%s\n", error), 1;

    const char *removed = dict->removeItem(dict, "two");
    printf("Removed: %s\n", removed ? removed : "(not found)");

    dict->clear(dict, cleanup);

    collection_dictionary_free(&dict);
    return 0;
}
