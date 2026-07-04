/**
 * @file example_dictionary_remove.c
 * @brief Dictionary Remove Item Example
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include "collection/i_dictionary.h"

#include <stdio.h>

int main(void) {
    struct IDictionary *dictionary = collection_dictionary_new();
    if (dictionary == NULL) return 1;

    dictionary->put(dictionary, "one", "1");
    dictionary->put(dictionary, "two", "2");
    dictionary->put(dictionary, "three", "3");

    const char *removed = dictionary->remove_item(dictionary, "two");
    printf("Removed: %s\n", removed ? removed : "(not found)");

    collection_dictionary_dealloc(&dictionary, NULL);
    return 0;
}
