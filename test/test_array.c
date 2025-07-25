#include <assert.h>
#include <stdlib.h>

#include "collection/collection.h"

struct Test {
    char *name;
    struct Test *next;
};

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

