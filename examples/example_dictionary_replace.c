/**
 * @file example_dictionary_replace.c
 * @brief Dictionary Replace Item Example
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include "collection/i_dictionary.h"

#include <stdio.h>

int main() {
    struct IDictionary *dictionary = collection_dictionary_new();

    dictionary->put(dictionary, "city", "New York");

    if (dictionary->contains_key(dictionary, "city")) {
        printf("City exists\n");
    }

    char *oldValue = dictionary->replace(dictionary, "city", "Los Angeles");
    printf("Old city: %s\n", oldValue ? oldValue : "(not replaced)");

    char *newValue = dictionary->get(dictionary, "city");
    printf("New city: %s\n", newValue ? newValue : "(not found)");

    collection_dictionary_dealloc(&dictionary, NULL);
}
