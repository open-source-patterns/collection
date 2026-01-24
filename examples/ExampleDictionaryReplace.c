#include <stdio.h>
#include "collection/IDictionary.h"

int main() {
    const char *error = NULL;
    struct IDictionary *dict = collection_dictionary_new(&error);
    if (error != NULL) return fprintf(stderr, "%s\n", error), 1;

    dict->put(dict, "city", "New York", &error);
    if (error != NULL) return fprintf(stderr, "%s\n", error), 1;

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
