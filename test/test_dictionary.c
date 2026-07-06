/**
 * @file test_dictionary.c
 * @brief Dictionary unit tests.
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include "test_dictionary.h"
#include "collection/i_dictionary.h"

#include <stdio.h>
#include <stdlib.h>

#pragma region Test Lifecycle

static void before_all(void) { }
static void before_each(void) { }
static void after_each(void) { }
static void after_all(void) { }

static void test(const char *name, void (*callback)(void)) {
    printf("\033[0;34m[RUNNING]\033[0m %s...\n", name);
    fflush(stdout);

    before_each();
    callback();
    after_each();

    printf("\033[0;32m[PASSED]\033[0m %s\n", name);
    fflush(stdout);
}

#pragma endregion

#pragma region Test Runner

int main(void) {
    printf("\n\033[1;36m================================================\033[0m\n");
    printf("\033[1;36m[SUITE] %s\033[0m\n", "DictionaryTest");
    printf("\033[1;36m================================================\033[0m\n\n");

    before_all();
    test("test_get", test_get);
    test("test_put", test_put);
    test("test_contains_key", test_contains_key);
    test("test_remove_item", test_remove_item);
    test("test_replace", test_replace);
    test("test_clear", test_clear);
    test("test_dealloc", test_dealloc);
    after_all();

    printf("\n\033[1;32m[DONE] All tests in suite finished.\033[0m\n");
    return 0;
}

#pragma endregion

#pragma region Types

struct Test {
    int value;
};

#pragma endregion

#pragma region Tests

// Verifies retrieving values by key.
void test_get(void) {
    struct IDictionary *dictionary = collection_dictionary_new();
    if (dictionary == NULL) abort();

    if (dictionary->get(dictionary, "key99") != NULL) abort();

    const struct Test *test1 = &(struct Test) {1};
    const struct Test *test2 = &(struct Test) {2};
    const struct Test *test3 = &(struct Test) {3};

    if (dictionary->put(dictionary, "key1", test1) != true) abort();
    if (dictionary->put(dictionary, "key2", test2) != true) abort();
    if (dictionary->put(dictionary, "key3", test3) != true) abort();

    if (dictionary->get(dictionary, "key1") != test1) abort();
    if (dictionary->get(dictionary, "key2") != test2) abort();
    if (dictionary->get(dictionary, "key3") != test3) abort();
    if (dictionary->get(dictionary, "key99") != NULL) abort();

    if (test1->value != 1) abort();
    if (test2->value != 2) abort();
    if (test3->value != 3) abort();

    collection_dictionary_dealloc(&dictionary, NULL);
}

// Verifies inserting key-value pairs.
void test_put(void) {
    struct IDictionary *dictionary = collection_dictionary_new();
    if (dictionary == NULL) abort();

    const struct Test *test1 = &(struct Test) {1};
    const struct Test *test2 = &(struct Test) {2};
    const struct Test *test3 = &(struct Test) {3};

    if (dictionary->put(dictionary, "key1", test1) != true) abort();
    if (dictionary->put(dictionary, "key2", test2) != true) abort();
    if (dictionary->put(dictionary, "key3", test3) != true) abort();

    if (dictionary->get(dictionary, "key1") != test1) abort();
    if (dictionary->get(dictionary, "key2") != test2) abort();
    if (dictionary->get(dictionary, "key3") != test3) abort();

    if (test1->value != 1) abort();
    if (test2->value != 2) abort();
    if (test3->value != 3) abort();

    collection_dictionary_dealloc(&dictionary, NULL);
}

// Verifies checking for key existence.
void test_contains_key(void) {
    struct IDictionary *dictionary = collection_dictionary_new();
    if (dictionary == NULL) abort();

    dictionary->put(dictionary, "key1", "test1");
    dictionary->put(dictionary, "key2", "test2");
    dictionary->put(dictionary, "key3", "test3");

    if (dictionary->contains_key(dictionary, "key1") != true) abort();
    if (dictionary->contains_key(dictionary, "key2") != true) abort();
    if (dictionary->contains_key(dictionary, "key3") != true) abort();
    if (dictionary->contains_key(dictionary, "invalid") != false) abort();

    collection_dictionary_dealloc(&dictionary, NULL);
}

// Verifies removing a key-value pair.
void test_remove_item(void) {
    struct IDictionary *dictionary = collection_dictionary_new();
    if (dictionary == NULL) abort();

    const struct Test *test1 = &(struct Test) {1};
    const struct Test *test2 = &(struct Test) {2};
    const struct Test *test3 = &(struct Test) {3};

    if (dictionary->put(dictionary, "key1", test1) != true) abort();
    if (dictionary->put(dictionary, "key2", test2) != true) abort();
    if (dictionary->put(dictionary, "key3", test3) != true) abort();

    if (dictionary->remove_item(dictionary, "key2") != test2) abort();
    if (dictionary->remove_item(dictionary, "key1") != test1) abort();
    if (dictionary->remove_item(dictionary, "key3") != test3) abort();
    if (dictionary->remove_item(dictionary, "key99") != NULL) abort();

    if (test1->value != 1) abort();
    if (test2->value != 2) abort();
    if (test3->value != 3) abort();

    collection_dictionary_dealloc(&dictionary, NULL);
}

// Verifies replacing an existing value.
void test_replace(void) {
    struct IDictionary *dictionary = collection_dictionary_new();
    if (dictionary == NULL) abort();

    const struct Test *test1 = &(struct Test) {1};
    const struct Test *test2 = &(struct Test) {2};
    const struct Test *test3 = &(struct Test) {3};

    if (dictionary->put(dictionary, "key1", test1) != true) abort();
    if (dictionary->put(dictionary, "key2", test2) != true) abort();
    if (dictionary->put(dictionary, "key3", test3) != true) abort();

    const struct Test *test4 = &(struct Test) {4};

    const struct Test *old_value = dictionary->replace(dictionary, "key3", test4);

    if (old_value != test3) abort();
    if (dictionary->contains_key(dictionary, "key3") != true) abort();

    if (dictionary->get(dictionary, "key3") != test4) abort();
    if (test4->value != 4) abort();

    collection_dictionary_dealloc(&dictionary, NULL);
}

// Verifies clearing all key-value pairs.
void test_clear(void) {
    struct IDictionary *dictionary = collection_dictionary_new();
    if (dictionary == NULL) abort();

    const struct Test *test1 = &(struct Test){1};
    const struct Test *test2 = &(struct Test){2};
    const struct Test *test3 = &(struct Test){3};

    dictionary->put(dictionary, "key1", test1);
    dictionary->put(dictionary, "key2", test2);
    dictionary->put(dictionary, "key3", test3);

    if (dictionary->clear(dictionary, NULL) != true) abort();

    if (dictionary->get(dictionary, "key1") != NULL) abort();
    if (dictionary->get(dictionary, "key2") != NULL) abort();
    if (dictionary->get(dictionary, "key3") != NULL) abort();

    if (dictionary->contains_key(dictionary, "key1") != false) abort();
    if (dictionary->contains_key(dictionary, "key2") != false) abort();
    if (dictionary->contains_key(dictionary, "key3") != false) abort();

    collection_dictionary_dealloc(&dictionary, NULL);
}

// Verifies destroying a dictionary and releasing resources.
void test_dealloc(void) {
    struct IDictionary *dictionary = collection_dictionary_new();
    if (dictionary == NULL) abort();

    struct Test *test1 = malloc(sizeof(struct Test));
    test1->value = 1;
    struct Test *test2 = malloc(sizeof(struct Test));
    test2->value = 2;
    struct Test *test3 = malloc(sizeof(struct Test));
    test3->value = 3;

    if (dictionary->put(dictionary, "key1", test1) != true) abort();
    if (dictionary->put(dictionary, "key2", test2) != true) abort();
    if (dictionary->put(dictionary, "key3", test3) != true) abort();

    // Pass free if stored items are heap allocated
    collection_dictionary_dealloc(&dictionary, free);
}

#pragma endregion
