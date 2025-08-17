#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "collection/IArray.h"

struct Test {
    char *name;
    struct Test *next;
};

struct Value {
    int value;
    struct Value *next;
};

static void testGet() {
    struct IArray *array = collection_array_new();

    array->push(array, &(struct Test){"name0"});
    array->push(array, &(struct Test){"name1"});
    array->push(array, &(struct Test){"name2"});
    array->push(array, &(struct Test){"name3"});
    array->push(array, &(struct Test){"name4"});

    assert(strcmp(((struct Test*)array->get(array, 0))->name, "name0") == 0);
    assert(strcmp(((struct Test*)array->get(array, 1))->name, "name1") == 0);
    assert(strcmp(((struct Test*)array->get(array, 2))->name, "name2") == 0);
    assert(strcmp(((struct Test*)array->get(array, 3))->name, "name3") == 0);
    assert(strcmp(((struct Test*)array->get(array, 4))->name, "name4") == 0);
}

static void testForEachCallback(const void *element, int index, const void *data) {
    struct Value *value = (struct Value *)element;
    const int *multiplier = data;
    value->value *= *multiplier;
}

void testForEach() {
    struct IArray *array = collection_array_new();

    array->push(array, &(struct Value){1});
    array->push(array, &(struct Value){2});
    array->push(array, &(struct Value){3});

    const int multiplier = 2;
    array->forEach(array, testForEachCallback, &multiplier);

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
    struct IArray *array = collection_array_new();

    array->push(array, &(struct Value){1});
    array->push(array, &(struct Value){2});
    array->push(array, &(struct Value){3});

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
    struct IArray *array = collection_array_new();

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    assert(test1->next == NULL);
    assert(test2->next == NULL);
    assert(test3->next == NULL);

    assert(array->unshift(array, test1) == test1);
    assert(array->unshift(array, test2) == test2);
    assert(array->unshift(array, test3) == test3);
}

void testAddLast() {
    struct IArray *array = collection_array_new();

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    assert(test1->next == NULL);
    assert(array->push(array, test1) == test1);
    assert(array->push(array, test2) == test2);
    assert(array->push(array, test3) == test3);

    assert(array->pop(array) == test3);
    assert(array->pop(array) == test2);
    assert(array->pop(array) == test1);
}

void testRemoveFirst() {
    struct IArray *array = collection_array_new();

    assert(array->shift(array) == NULL);

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    array->push(array, test1);
    array->push(array, test2);
    array->push(array, test3);

    assert(array->shift(array) == test1);
    assert(array->shift(array) == test2);
    assert(array->shift(array) == test3);
    assert(array->shift(array) == NULL);
}

void testRemoveLast() {
    struct IArray *array = collection_array_new();

    assert(array->pop(array) == NULL);

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    array->push(array, test1);
    array->push(array, test2);
    array->push(array, test3);

    assert(array->pop(array) == test3);
    assert(array->pop(array) == test2);
    assert(array->pop(array) == test1);
    assert(array->pop(array) == NULL);
}

void testRemove() {
    struct IArray *array = collection_array_new();

    assert(array->removeItem(array, NULL) == NULL);
    assert(array->removeItem(array, &(struct Test) {"name4"}) == NULL);

    const struct Test *test1 = &(struct Test) {"name"};
    const struct Test *test2 = &(struct Test) {"name"};
    const struct Test *test3 = &(struct Test) {"name"};

    array->push(array, test1);
    array->push(array, test2);
    array->push(array, test3);

    assert(array->removeItem(array, test2) == test2);
    assert(array->removeItem(array, test1) == test1);
    assert(array->removeItem(array, test3) == test3);
    assert(array->removeItem(array, &(struct Test) {"name4"}) == NULL);
}

void testContainsValue() {
    struct IArray *array = collection_array_new();

    assert(array->containsValue(array, NULL) == false);
    assert(array->containsValue(array, "emptyList") == false);

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    array->push(array, test1);
    array->push(array, test2);
    array->push(array, test3);

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
    struct IArray *array = collection_array_new();

    assert(array->clone(array)->size(array) == 0);

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    array->push(array, test1);
    array->push(array, test2);
    array->push(array, test3);

    struct IArray *clone = array->clone(array);

    array->clear(array, NULL);
    assert(array->size(array) == 0);

    assert(clone->pop(clone) == test3);
    assert(clone->pop(clone) == test2);
    assert(clone->pop(clone) == test1);
}

void testSize() {
    struct IArray *array = collection_array_new();

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    array->push(array, test1);
    assert(array->size(array) == 1);

    array->push(array, test2);
    assert(array->size(array) == 2);

    array->push(array, test3);
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
    struct IArray *array = collection_array_new();

    array->clear(array, NULL);

    const struct Test *test1 = &(struct Test) {"name1"};
    const struct Test *test2 = &(struct Test) {"name2"};
    const struct Test *test3 = &(struct Test) {"name3"};

    array->push(array, test1);
    array->push(array, test2);
    array->push(array, test3);

    array->clear(array, NULL);
    assert(array->size(array) == 0);
}

void testDealloc() {
    struct IArray *array = collection_array_new();

    struct Test *test1 = malloc(sizeof(struct Test));
    test1->name = "name1";
    struct Test *test2 = malloc(sizeof(struct Test));
    test2->name = "name2";
    struct Test *test3 = malloc(sizeof(struct Test));
    test3->name = "name3";

    assert(array->push(array, test1) == test1);
    assert(array->push(array, test2) == test2);
    assert(array->push(array, test3) == test3);

    // Clear the array first if you have dynamically allocated memory items in it
    array->clear(array, free); // can pass NULL as the callback for static values

    collection_array_free(&array);
    assert(array == NULL);
}

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

