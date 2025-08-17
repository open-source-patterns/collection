#include <stdio.h>
#include "collection/IDictionary.h"

void cleanup(void *value) {
    printf("Freeing value: %s\n", (char *)value);
    // Only free if dynamically allocated
}

int main() {
    struct IDictionary *dict = collection_dictionary_new();

    dict->put(dict, "one", "1");
    dict->put(dict, "two", "2");
    dict->put(dict, "three", "3");

    const char *removed = dict->removeItem(dict, "two");
    printf("Removed: %s\n", removed ? removed : "(not found)");

    dict->clear(dict, cleanup);

    collection_dictionary_free(&dict);
    return 0;
}
