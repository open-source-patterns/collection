/**
 * @file example_dictionary_replace.c
 * @brief Dictionary Replace Item Example
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include "collection/i_dictionary.h"

#include <stdio.h>

int main(void) {
    struct IDictionary *dictionary = collection_dictionary_new();
    if (dictionary == NULL) return 1;

    dictionary->put(dictionary, "city", "New York");

    if (dictionary->contains_key(dictionary, "city")) {
        printf("City exists\n");
    }

    const char *oldValue = dictionary->replace(dictionary, "city", "Los Angeles");
    printf("Old city: %s\n", oldValue ? oldValue : "(not replaced)");

    const char *newValue = dictionary->get(dictionary, "city");
    printf("New city: %s\n", newValue ? newValue : "(not found)");

    collection_dictionary_dealloc(&dictionary, NULL);
}
