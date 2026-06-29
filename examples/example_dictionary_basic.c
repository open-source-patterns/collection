/**
 * @file example_dictionary_basic.c
 * @brief Basic Dictionary Example
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include "collection/i_dictionary.h"

#include <stdio.h>

void dictionary_usage1();
void dictionary_usage2();

int main() {
    dictionary_usage1();
    dictionary_usage2();
    return 0;
}

void dictionary_usage1() {
    struct IDictionary *dictionary = collection_dictionary_new();;

    dictionary->put(dictionary, "name", "Alice");
    dictionary->put(dictionary, "age", "30");

    char *name = dictionary->get(dictionary, "name");
    char *age = dictionary->get(dictionary, "age");

    printf("Name: %s\n", name ? name : "(not found)");
    printf("Age: %s\n", age ? age : "(not found)");

    collection_dictionary_dealloc(&dictionary, NULL);
}

void dictionary_usage2() {
    struct IDictionary *dictionary = collection_dictionary_new();;

    dictionary->put(dictionary, "name", "Bob");
    dictionary->put(dictionary, "age", "31");

    char *name = dictionary->get(dictionary, "name");
    printf("Name: %s\n", (const char *) &name); // Name: Bob
    dictionary->remove_item(dictionary, "age");

    collection_dictionary_dealloc(&dictionary, NULL);
}