/**
* @file ExampleDictionaryBasic.c
 * @brief Basic Dictionary Example
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include <stdio.h>
#include "collection/IDictionary.h"

void dictionary_usage();

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

    dictionary_usage();

    return 0;
}

void dictionary_usage() {
    const char *error = NULL;
    struct IDictionary *dict = collection_dictionary_new(&error);
    dict->put(dict, "name", "Alice", &error);
    dict->put(dict, "age", "30", &error);

    printf("Name: %s\n", (const char *)dict->get(dict, "name")); // Name: Alice
    dict->removeItem(dict, "age");

    collection_dictionary_free(&dict);
}