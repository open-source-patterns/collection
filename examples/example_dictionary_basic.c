/**
 * @file example_dictionary_basic.c
 * @brief Basic Dictionary Example
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include "collection/i_dictionary.h"

#include <stdio.h>

static void dictionary_usage1(void);
static void dictionary_usage2(void);

int main(void) {
    dictionary_usage1();
    dictionary_usage2();
    return 0;
}

static void dictionary_usage1(void) {
    struct IDictionary *dictionary = collection_dictionary_new();
    if (dictionary == NULL) return;

    dictionary->put(dictionary, "name", "Alice");
    dictionary->put(dictionary, "age", "30");

    printf("Name: %s\n", (const char *) dictionary->get(dictionary, "name")); // Name: Alice
    printf("Age: %s\n", (const char *) dictionary->get(dictionary, "age")); // Age: 30

    collection_dictionary_dealloc(&dictionary, NULL);
}

static void dictionary_usage2(void) {
    struct IDictionary *dictionary = collection_dictionary_new();
    if (dictionary == NULL) return;

    dictionary->put(dictionary, "name", "Bob");
    dictionary->put(dictionary, "age", "31");

    const char *name = dictionary->get(dictionary, "name");
    printf("Name: %s\n", name ? name : "(not found)");
    dictionary->remove_item(dictionary, "age");

    printf("Age: %s\n", dictionary->contains_key(dictionary, "age") ? "found" : "(not found)");

    collection_dictionary_dealloc(&dictionary, NULL);
}
