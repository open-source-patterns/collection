/**
 * @file test_array.c
 * @brief Array Unit Test
 *
 * @author Saad Shams https://linkedin.com/in/muizz
 * @copyright BSD 3-Clause License
 */
#include "test_array.h"
#include "collection/i_array.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static void before_all(void) {}
static void before_each(void) {}
static void after_each(void) {}
static void after_all(void) {}

static void test(const char *name, void (*callback)(void)) {
    printf("\033[0;34m[RUNNING]\033[0m %s...\n", name);
    fflush(stdout);

    before_each();
    callback();
    after_each();

    printf("\033[0;32m[PASSED]\033[0m %s\n", name);
    fflush(stdout);
}

int main(void) {
    printf("\n\033[1;36m================================================\033[0m\n");
    printf("\033[1;36m[SUITE] %s\033[0m\n", "ArrayTest");
    printf("\033[1;36m================================================\033[0m\n\n");

    before_all();
    test("test_get", test_get);
    test("test_put", test_put);
    test("test_for_each", test_for_each);
    test("test_find", test_find);
    test("test_first_index", test_first_index);
    test("test_last_index", test_last_index);
    test("test_add_first", test_add_first);
    test("test_add_last", test_add_last);
    test("test_remove_first", test_remove_first);
    test("test_remove_last", test_remove_last);
    test("test_remove", test_remove);
    test("test_contains_value", test_contains_value);
    test("test_clone", test_clone);
    test("test_size", test_size);
    test("test_clear", test_clear);
    test("test_dealloc", test_dealloc);
    after_all();

    printf("\n\033[1;32m[DONE] All tests in suite finished.\033[0m\n");
    return 0;
}

struct Test {
    char *name;
};

struct Value {
    int value;
};

void test_get(void) {
    struct IArray *array = collection_array_new();
    if (array == NULL) abort();

    array->push(array, &(struct Test){"name0"});
    array->push(array, &(struct Test){"name1"});
    array->push(array, &(struct Test){"name2"});

    if (strcmp(((struct Test*) array->get(array, 0))->name, "name0") != 0) abort();
    if (strcmp(((struct Test*) array->get(array, 1))->name, "name1") != 0) abort();
    if (strcmp(((struct Test*) array->get(array, 2))->name, "name2") != 0) abort();

    collection_array_dealloc(&array, NULL);
}

void test_put(void) {
    struct IArray *array = collection_array_new();
    if (array == NULL) abort();

    array->push(array, &(struct Test){"name0"});
    array->push(array, &(struct Test){"name1"});
    array->push(array, &(struct Test){"name2"});

    array->put(array, &(struct Test){"name3"}, 0);
    array->put(array, &(struct Test){"name4"}, 1);
    array->put(array, &(struct Test){"name5"}, 2);

    if (strcmp(((struct Test*) array->get(array, 0))->name, "name3") != 0) abort();
    if (strcmp(((struct Test*) array->get(array, 1))->name, "name4") != 0) abort();
    if (strcmp(((struct Test*) array->get(array, 2))->name, "name5") != 0) abort();

    collection_array_dealloc(&array, NULL);
}

static void testForEachCallback(const void *element, const void *item) {
    struct Value *value = (struct Value *)element;
    const int *multiplier = item;
    value->value *= *multiplier;
}

void test_for_each(void) {
    struct IArray *array = collection_array_new();
    if (array == NULL) abort();

    array->push(array, &(struct Value){1});
    array->push(array, &(struct Value){2});
    array->push(array, &(struct Value){3});

    const int multiplier = 2;
    array->for_each(array, testForEachCallback, &multiplier);

    if (((struct Value *) array->shift(array))->value != 2) abort();
    if (((struct Value *) array->shift(array))->value != 4) abort();
    if (((struct Value *) array->shift(array))->value != 6) abort();

    collection_array_dealloc(&array, NULL);
}

static bool comparator_find(const void *element, const void *item) {
    return ((struct Value *) element)->value == ((struct Value *) item)->value;
}

void test_find(void) {
    struct IArray *array = collection_array_new();
    if (array == NULL) abort();

    array->push(array, &(struct Value){1});
    array->push(array, &(struct Value){2});
    array->push(array, &(struct Value){3});

    const struct Value *value = (struct Value *) array->find(array, comparator_find, &(struct Value){1});
    if (value->value != 1) abort();
    value = (struct Value *) array->find(array, comparator_find, &(struct Value){2});
    if (value->value != 2) abort();
    value = (struct Value *) array->find(array, comparator_find, &(struct Value){3});
    if (value->value != 3) abort();
    value = (struct Value *) array->find(array, comparator_find, &(struct Value){4});
    if (value != NULL) abort();

    collection_array_dealloc(&array, NULL);
}

static bool comparator_first_index(const void *element, const void *item) {
    return ((struct Value *) element)->value == ((struct Value *) item)->value;
}

void test_first_index(void) {
    struct IArray *array = collection_array_new();
    if (array == NULL) abort();

    array->push(array, &(struct Value){1});
    array->push(array, &(struct Value){2});
    array->push(array, &(struct Value){3});

    size_t index = array->first_index(array, comparator_first_index, &(struct Value){1});
    if (index != 0) abort();
    index = array->first_index(array, comparator_first_index, &(struct Value){2});
    if (index != 1) abort();
    index = array->first_index(array, comparator_first_index, &(struct Value){3});
    if (index != 2) abort();

    collection_array_dealloc(&array, NULL);
}

static bool comparator_last_index(const void *element, const void *item) {
    return ((struct Value *) element)->value == ((struct Value *) item)->value;
}

void test_last_index(void) {
    struct IArray *array = collection_array_new();
    if (array == NULL) abort();

    array->push(array, &(struct Value){1});
    array->push(array, &(struct Value){1});
    array->push(array, &(struct Value){2});
    array->push(array, &(struct Value){2});
    array->push(array, &(struct Value){2});
    array->push(array, &(struct Value){3});
    array->push(array, &(struct Value){3});
    array->push(array, &(struct Value){3});

    size_t index = array->last_index(array, comparator_last_index, &(struct Value){1});
    if (index != 1) abort();
    index = array->last_index(array, comparator_last_index, &(struct Value){2});
    if (index != 4) abort();
    index = array->last_index(array, comparator_last_index, &(struct Value){3});
    if (index != 7) abort();

    collection_array_dealloc(&array, NULL);
}

void test_add_first(void) {
    struct IArray *array = collection_array_new();
    if (array == NULL) abort();

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    if (array->unshift(array, test1) != test1) abort();
    if (array->unshift(array, test2) != test2) abort();
    if (array->unshift(array, test3) != test3) abort();

    collection_array_dealloc(&array, NULL);
}

void test_add_last(void) {
    struct IArray *array = collection_array_new();
    if (array == NULL) abort();

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    if (array->push(array, test1) != true) abort();
    if (array->push(array, test2) != true) abort();
    if (array->push(array, test3) != true) abort();

    if (array->pop(array) != test3) abort();
    if (array->pop(array) != test2) abort();
    if (array->pop(array) != test1) abort();

    collection_array_dealloc(&array, NULL);
}

void test_remove_first(void) {
    struct IArray *array = collection_array_new();
    if (array == NULL) abort();

    if (array->shift(array) != NULL) abort();

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    array->push(array, test1);
    array->push(array, test2);
    array->push(array, test3);

    if (array->shift(array) != test1) abort();
    if (array->shift(array) != test2) abort();
    if (array->shift(array) != test3) abort();
    if (array->shift(array) != NULL) abort();

    collection_array_dealloc(&array, NULL);
}

void test_remove_last(void) {
    struct IArray *array = collection_array_new();
    if (array == NULL) abort();

    if (array->pop(array) != NULL) abort();

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    array->push(array, test1);
    array->push(array, test2);
    array->push(array, test3);

    if (array->pop(array) != test3) abort();
    if (array->pop(array) != test2) abort();
    if (array->pop(array) != test1) abort();
    if (array->pop(array) != NULL) abort();

    collection_array_dealloc(&array, NULL);
}

void test_remove(void) {
    struct IArray *array = collection_array_new();
    if (array == NULL) abort();

    if (array->remove_item(array, NULL) != NULL) abort();
    if (array->remove_item(array, &(struct Test) {"name4"}) != NULL) abort();

    const struct Test *test1 = &(struct Test) {"name"};
    const struct Test *test2 = &(struct Test) {"name"};
    const struct Test *test3 = &(struct Test) {"name"};

    array->push(array, test1);
    array->push(array, test2);
    array->push(array, test3);

    if (array->remove_item(array, test2) != test2) abort();
    if (array->remove_item(array, test1) != test1) abort();
    if (array->remove_item(array, test3) != test3) abort();
    if (array->remove_item(array, &(struct Test) {"name4"}) != NULL) abort();

    collection_array_dealloc(&array, NULL);
}

void test_contains_value(void) {
    struct IArray *array = collection_array_new();
    if (array == NULL) abort();

    if (array->contains_value(array, NULL) != false) abort();
    if (array->contains_value(array, "emptyList") != false) abort();

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    array->push(array, test1);
    array->push(array, test2);
    array->push(array, test3);

    if (array->contains_value(array, test1) != true) abort();
    if (array->contains_value(array, test2) != true) abort();
    if (array->contains_value(array, test3) != true) abort();
    if (array->contains_value(array, &(struct Test) {"name4"}) != false) abort();

    array->remove_item(array, test2);
    if (array->contains_value(array, test2) != false) abort();

    array->remove_item(array, test3);
    if (array->contains_value(array, test3) != false) abort();

    array->remove_item(array, test1);
    if (array->contains_value(array, test1) != false) abort();

    collection_array_dealloc(&array, NULL);
}

void test_clone(void) {
    struct IArray *array = collection_array_new();
    if (array == NULL) abort();

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    array->push(array, test1);
    array->push(array, test2);
    array->push(array, test3);

    struct IArray *clone = array->clone(array);

    array->clear(array, NULL);
    if (array->count(array) != 0) abort();

    if (clone->pop(clone) != test3) abort();
    if (clone->pop(clone) != test2) abort();
    if (clone->pop(clone) != test1) abort();

    collection_array_dealloc(&array, NULL);

    collection_array_dealloc(&clone, NULL);
}

void test_size(void) {
    struct IArray *array = collection_array_new();
    if (array == NULL) abort();

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    array->push(array, test1);
    if (array->count(array) != 1) abort();

    array->push(array, test2);
    if (array->count(array) != 2) abort();

    array->push(array, test3);
    if (array->count(array) != 3) abort();

    array->remove_item(array, test2);
    if (array->count(array) != 2) abort();

    array->remove_item(array, test1);
    array->remove_item(array, test2);
    if (array->count(array) != 1) abort();

    array->remove_item(array, test3);
    if (array->count(array) != 0) abort();

    collection_array_dealloc(&array, NULL);
}

void test_clear(void) {
    struct IArray *array = collection_array_new();
    if (array == NULL) abort();

    array->clear(array, NULL);

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    array->push(array, test1);
    array->push(array, test2);
    array->push(array, test3);

    array->clear(array, NULL);
    if (array->count(array) != 0) abort();

    collection_array_dealloc(&array, NULL);
}

void test_dealloc(void) {
    struct IArray *array = collection_array_new();
    if (array == NULL) abort();

    struct Test *test1 = malloc(sizeof(struct Test));
    test1->name = "name1";
    struct Test *test2 = malloc(sizeof(struct Test));
    test2->name = "name2";
    struct Test *test3 = malloc(sizeof(struct Test));
    test3->name = "name3";

    if (array->push(array, test1) != true) abort();
    if (array->push(array, test2) != true) abort();
    if (array->push(array, test3) != true) abort();

    // Pass free if stored items are heap allocated
    collection_array_dealloc(&array, free);
}
