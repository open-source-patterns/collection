/**
 * @file DictionaryTest.c
 * @brief Dictionary Unit Test
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include <assert.h>
#include <stdlib.h>

#include "collection/IDictionary.h"
#include "DictionaryTest.h"

int main() {
    testContainsKey();
    testPut();
    testGet();
    testReplace();
    testDealloc();
}

struct Test {
    int value;
};

void testContainsKey() {
    const char *error = NULL;
    struct IDictionary *dictionary = collection_dictionary_new(&error);

    dictionary->put(dictionary, "key1", "test1", &error);
    dictionary->put(dictionary, "key2", "test2", &error);
    dictionary->put(dictionary, "key3", "test3", &error);

    assert(dictionary->containsKey(dictionary, "key1") == true);
    assert(dictionary->containsKey(dictionary, "key2") == true);
    assert(dictionary->containsKey(dictionary, "key3") == true);
    assert(dictionary->containsKey(dictionary, "invalid") == false);

    collection_dictionary_free(&dictionary);
}

void testPut() {
    const char *error = NULL;
    struct IDictionary *dictionary = collection_dictionary_new(&error);

    const struct Test *test1 = &(struct Test) {1};
    const struct Test *test2 = &(struct Test) {2};
    const struct Test *test3 = &(struct Test) {3};

    assert(dictionary->put(dictionary, "key1", test1, &error) == true);
    assert(dictionary->put(dictionary, "key2", test2, &error) == true);
    assert(dictionary->put(dictionary, "key3", test3, &error) == true);

    assert(dictionary->get(dictionary, "key1") == test1);
    assert(dictionary->get(dictionary, "key2") == test2);
    assert(dictionary->get(dictionary, "key3") == test3);

    collection_dictionary_free(&dictionary);
}

void testGet() {
    const char *error = NULL;
    struct IDictionary *dictionary = collection_dictionary_new(&error);
    assert(dictionary->get(dictionary, "unknown") == NULL);

    const struct Test *test1 = &(struct Test) {1};
    const struct Test *test2 = &(struct Test) { 2};
    const struct Test *test3 = &(struct Test) {3};

    assert(dictionary->put(dictionary, "key1", test1, &error) == true);
    assert(dictionary->put(dictionary, "key2", test2, &error) == true);
    assert(dictionary->put(dictionary, "key3", test3, &error) == true);

    assert(dictionary->get(dictionary, "key1") == test1);
    assert(dictionary->get(dictionary, "key2") == test2);
    assert(dictionary->get(dictionary, "key3") == test3);
    assert(dictionary->get(dictionary, "unknown") == NULL);
}

void testReplace() {
    const char *error = NULL;
    struct IDictionary *dictionary = collection_dictionary_new(&error);

    const struct Test *test1 = &(struct Test) {1};
    const struct Test *test2 = &(struct Test) { 2};
    const struct Test *test3 = &(struct Test) {3};

    assert(dictionary->put(dictionary, "key1", test1, &error) == true);
    assert(dictionary->put(dictionary, "key2", test2, &error) == true);
    assert(dictionary->put(dictionary, "key3", test3, &error) == true);

    const struct Test *test4 = &(struct Test) {4};

    const struct Test *oldValue = (struct Test *) (dictionary->replace(dictionary, "key3", test4));

    assert(oldValue == test3);
    assert(dictionary->containsKey(dictionary, "key3") == true);

    assert(dictionary->get(dictionary, "key3") == test4);
    assert(&test3 != NULL);
}

void testRemove() {
    const char *error = NULL;
    struct IDictionary *dictionary = collection_dictionary_new(&error);

    const struct Test *test1 = &(struct Test) {1};
    const struct Test *test2 = &(struct Test) { 2};
    const struct Test *test3 = &(struct Test) {3};

    assert(dictionary->put(dictionary, "key1", test1, &error) == true);
    assert(dictionary->put(dictionary, "key2", test2, &error) == true);
    assert(dictionary->put(dictionary, "key3", test3, &error) == true);

    assert(dictionary->removeItem(dictionary, "key2") == test2);
    assert(dictionary->removeItem(dictionary, "key1") == test1);
    assert(dictionary->removeItem(dictionary, "key3") == test3);
    assert(dictionary->removeItem(dictionary, "key4") == NULL);
}

void testDealloc() {
    const char *error = NULL;
    struct IDictionary *dictionary = collection_dictionary_new(&error);

    struct Test *test1 = malloc(sizeof(struct Test));
    test1->value = 1;
    struct Test *test2 = malloc(sizeof(struct Test));
    test2->value = 2;
    struct Test *test3 = malloc(sizeof(struct Test));
    test3->value = 3;

    assert(dictionary->put(dictionary, "key1", test1, &error) == true);
    assert(dictionary->put(dictionary, "key2", test2, &error) == true);
    assert(dictionary->put(dictionary, "key3", test3, &error) == true);

    // Clear the dictionary first if you have dynamically allocated memory items in it
    dictionary->clear(dictionary, free); // can pass NULL as the callback for static values

    collection_dictionary_free(&dictionary);
    assert(dictionary == NULL);
}
