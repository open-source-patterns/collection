#include <stdio.h>
#include "collection/IDictionary.h"

int main() {
    struct IDictionary *dict = collection_dictionary_new();

    dict->put(dict, "city", "New York");

    if (dict->containsKey(dict, "city")) {
        printf("City exists\n");
    }

    const char *oldValue = dict->replace(dict, "city", "Los Angeles");
    printf("Old city: %s\n", oldValue ? oldValue : "(not replaced)");

    const char *newValue = dict->get(dict, "city");
    printf("New city: %s\n", newValue ? newValue : "(not found)");

    collection_dictionary_free(&dict);
    return 0;
}
