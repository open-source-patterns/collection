/**
* @file ExampleDictionaryBasic.c
 * @brief Basic Dictionary Example
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include <stdio.h>
#include "collection/IDictionary.h"

int main() {
    const char *error = NULL;
    struct IDictionary *dict = collection_dictionary_new(&error);
    if (error != NULL) return fprintf(stderr, "%s\n", error), 1;

    dict->put(dict, "name", "Alice", &error);
    if (error != NULL) return fprintf(stderr, "%s\n", error), 1;
    dict->put(dict, "age", "30", &error);
    if (error != NULL) return fprintf(stderr, "%s\n", error), 1;

    const char *name = dict->get(dict, "name");
    const char *age = dict->get(dict, "age");

    printf("Name: %s\n", name ? name : "(not found)");
    printf("Age: %s\n", age ? age : "(not found)");

    collection_dictionary_free(&dict);
    return 0;
}
