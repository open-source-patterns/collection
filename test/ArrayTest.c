#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "collection/IArray.h"
#include "ArrayTest.h"

int main() {
    testGet();
    testForEach();
    testFind();
    testAddFirst();
    testAddLast();
    testRemoveFirst();
    testRemoveLast();
    testRemove();
    testContainsValue();
    testClone();
    testSize();
    testClear();
    testDealloc();
    return 0;
}

struct Test {
    char *name;
    struct Test *next;
};

struct Value {
    int value;
    struct Value *next;
};

void testGet() {
    const char *error = NULL;
    struct IArray *array = collection_array_new(&error);

    array->push(array, &(struct Test){"name0"}, &error);
    array->push(array, &(struct Test){"name1"}, &error);
    array->push(array, &(struct Test){"name2"}, &error);
    array->push(array, &(struct Test){"name3"}, &error);
    array->push(array, &(struct Test){"name4"}, &error);

    assert(strcmp(((struct Test*)array->get(array, 0))->name, "name0") == 0);
    assert(strcmp(((struct Test*)array->get(array, 1))->name, "name1") == 0);
    assert(strcmp(((struct Test*)array->get(array, 2))->name, "name2") == 0);
    assert(strcmp(((struct Test*)array->get(array, 3))->name, "name3") == 0);
    assert(strcmp(((struct Test*)array->get(array, 4))->name, "name4") == 0);
}

static void testForEachCallback(const void *element, const void *data, const char **error) {
    struct Value *value = (struct Value *)element;
    const int *multiplier = data;
    value->value *= *multiplier;
}

void testForEach() {
    const char *error = NULL;
    struct IArray *array = collection_array_new(&error);

    array->push(array, &(struct Value){1}, &error);
    array->push(array, &(struct Value){2}, &error);
    array->push(array, &(struct Value){3}, &error);

    const int multiplier = 2;
    array->forEach(array, testForEachCallback, &multiplier, &error);

    assert(((struct Value *)array->shift(array))->value == 2);
    assert(((struct Value *)array->shift(array))->value == 4);
    assert(((struct Value *)array->shift(array))->value == 6);

    collection_array_free(&array);
}

static bool testFindCallback(const void *element, const void *data) {
    const struct Value *value1 = element;
    const struct Value *value2 = data;
    return value1->value == value2->value;
}

void testFind() {
    const char *error = NULL;
    struct IArray *array = collection_array_new(&error);

    array->push(array, &(struct Value){1}, &error);
    array->push(array, &(struct Value){2}, &error);
    array->push(array, &(struct Value){3}, &error);

    const struct Value *value = (struct Value *)array->find(array, testFindCallback, &(struct Value){1});
    assert(value->value == 1);
    value = (struct Value *) array->find(array, testFindCallback, &(struct Value){2});
    assert(value->value == 2);
    value = (struct Value *) array->find(array, testFindCallback, &(struct Value){3});
    assert(value->value == 3);
    value = (struct Value *) array->find(array, testFindCallback, &(struct Value){4});
    assert(value == NULL);

    collection_array_free(&array);
}

void testAddFirst() {
    const char *error = NULL;
    struct IArray *array = collection_array_new(&error);

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    assert(test1->next == NULL);
    assert(test2->next == NULL);
    assert(test3->next == NULL);

    assert(array->unshift(array, test1, &error) == test1);
    assert(array->unshift(array, test2, &error) == test2);
    assert(array->unshift(array, test3, &error) == test3);
}

void testAddLast() {
    const char *error = NULL;
    struct IArray *array = collection_array_new(&error);

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    assert(test1->next == NULL);
    assert(array->push(array, test1, &error) == test1);
    assert(array->push(array, test2, &error) == test2);
    assert(array->push(array, test3, &error) == test3);

    assert(array->pop(array) == test3);
    assert(array->pop(array) == test2);
    assert(array->pop(array) == test1);
}

void testRemoveFirst() {
    const char *error = NULL;
    struct IArray *array = collection_array_new(&error);

    assert(array->shift(array) == NULL);

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    array->push(array, test1, &error);
    array->push(array, test2, &error);
    array->push(array, test3, &error);

    assert(array->shift(array) == test1);
    assert(array->shift(array) == test2);
    assert(array->shift(array) == test3);
    assert(array->shift(array) == NULL);
}

void testRemoveLast() {
    const char *error = NULL;
    struct IArray *array = collection_array_new(&error);

    assert(array->pop(array) == NULL);

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    array->push(array, test1, &error);
    array->push(array, test2, &error);
    array->push(array, test3, &error);

    assert(array->pop(array) == test3);
    assert(array->pop(array) == test2);
    assert(array->pop(array) == test1);
    assert(array->pop(array) == NULL);
}

void testRemove() {
    const char *error = NULL;
    struct IArray *array = collection_array_new(&error);

    assert(array->removeItem(array, NULL) == NULL);
    assert(array->removeItem(array, &(struct Test) {"name4"}) == NULL);

    const struct Test *test1 = &(struct Test) {"name"};
    const struct Test *test2 = &(struct Test) {"name"};
    const struct Test *test3 = &(struct Test) {"name"};

    array->push(array, test1, &error);
    array->push(array, test2, &error);
    array->push(array, test3, &error);

    assert(array->removeItem(array, test2) == test2);
    assert(array->removeItem(array, test1) == test1);
    assert(array->removeItem(array, test3) == test3);
    assert(array->removeItem(array, &(struct Test) {"name4"}) == NULL);
}

void testContainsValue() {
    const char *error = NULL;
    struct IArray *array = collection_array_new(&error);

    assert(array->containsValue(array, NULL) == false);
    assert(array->containsValue(array, "emptyList") == false);

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    array->push(array, test1, &error);
    array->push(array, test2, &error);
    array->push(array, test3, &error);

    assert(array->containsValue(array, test1) == true);
    assert(array->containsValue(array, test2) == true);
    assert(array->containsValue(array, test3) == true);
    assert(array->containsValue(array, &(struct Test) {"name4"}) == false);

    array->removeItem(array, test2);
    assert(array->containsValue(array, test2) == false);

    array->removeItem(array, test3);
    assert(array->containsValue(array, test3) == false);

    array->removeItem(array, test1);
    assert(array->containsValue(array, test1) == false);
}

void testClone() {
    const char *error = NULL;
    struct IArray *array = collection_array_new(&error);

    assert(array->clone(array, &error)->size(array) == 0);

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    array->push(array, test1, &error);
    array->push(array, test2, &error);
    array->push(array, test3, &error);

    struct IArray *clone = array->clone(array, &error);

    array->clear(array, NULL);
    assert(array->size(array) == 0);

    assert(clone->pop(clone) == test3);
    assert(clone->pop(clone) == test2);
    assert(clone->pop(clone) == test1);
}

void testSize() {
    const char *error = NULL;
    struct IArray *array = collection_array_new(&error);

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    array->push(array, test1, &error);
    assert(array->size(array) == 1);

    array->push(array, test2, &error);
    assert(array->size(array) == 2);

    array->push(array, test3, &error);
    assert(array->size(array) == 3);

    array->removeItem(array, test2);
    assert(array->size(array) == 2);

    array->removeItem(array, test1);
    array->removeItem(array, test2);
    assert(array->size(array) == 1);

    array->removeItem(array, test3);
    assert(array->size(array) == 0);
}

void testClear() {
    const char *error = NULL;
    struct IArray *array = collection_array_new(&error);

    array->clear(array, NULL);

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    array->push(array, test1, &error);
    array->push(array, test2, &error);
    array->push(array, test3, &error);

    array->clear(array, NULL);
    assert(array->size(array) == 0);
}

void testDealloc() {
    const char *error = NULL;
    struct IArray *array = collection_array_new(&error);

    struct Test *test1 = malloc(sizeof(struct Test));
    test1->name = "name1";
    struct Test *test2 = malloc(sizeof(struct Test));
    test2->name = "name2";
    struct Test *test3 = malloc(sizeof(struct Test));
    test3->name = "name3";

    assert(array->push(array, test1, &error) == test1);
    assert(array->push(array, test2, &error) == test2);
    assert(array->push(array, test3, &error) == test3);

    // Clear the array first if you have dynamically allocated memory items in it
    array->clear(array, free); // can pass NULL as the callback for static values

    collection_array_free(&array);
    assert(array == NULL);
}
