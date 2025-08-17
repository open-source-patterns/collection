#include <stdio.h>
#include "collection/IDictionary.h"

int main() {
    struct IDictionary *dict = collection_dictionary_new();
    if (!dict) {
        fprintf(stderr, "Failed to create dictionary\n");
        return 1;
    }

    dict->put(dict, "name", "Alice");
    dict->put(dict, "age", "30");

    const char *name = dict->get(dict, "name");
    const char *age = dict->get(dict, "age");

    printf("Name: %s\n", name ? name : "(not found)");
    printf("Age: %s\n", age ? age : "(not found)");

    collection_dictionary_free(&dict);
    return 0;
}
