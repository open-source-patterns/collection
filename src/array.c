#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "collection/array.h"
#include "collection/mutex.h"

/**
 * @struct ArrayNode
 * @brief Represents a ArrayNode in a singly linked list.
 *
 * Each ArrayNode stores a generic pointer to an item and a pointer to the next ArrayNode in the list.
 */
struct ArrayNode {
    const void *item;               /**< 📦 Pointer to the stored item (generic type) */
    struct ArrayNode *next;         /**< ➡️ Pointer to the next ArrayNode in the singly linked list */
};

/**
 * @struct Array
 * @brief Represents a linked-list-based array structure.
 *
 * Contains a base interface (IArray) and a pointer to the head ArrayNode of the singly linked list.
 */
struct Array {
    struct IArray base;             /**< 🧩 Base interface for array operations */
    struct ArrayNode *list;         /**< 🗃️ Head of the singly linked ArrayNode */
};

// 🔒 Mutex protecting the linked ArrayNode for thread safety
static Mutex mutex;

// ➕ Insert item at head (unshift) with write-lock
static const void *unshift(struct IArray *self, const void *item) {
    struct ArrayNode *ArrayNode = malloc(sizeof(struct ArrayNode));
    if (ArrayNode == NULL) {
        fprintf(stderr, "ArrayNode allocation failed.\n");
        return NULL;
    }

    struct Array *instance = (struct Array *) self;

    mutex_lock(&mutex);
    ArrayNode->item = item;              // 🆕 Store item
    ArrayNode->next = instance->list;    // 🔗 Link old head
    instance->list = ArrayNode;          // 🔄 Update head pointer
    mutex_unlock(&mutex);

    return item;
}

// ➕ Append item at tail (push) with write-lock
static const void *push(struct IArray *self, const void *item) {
    struct ArrayNode *ArrayNode = malloc(sizeof(struct ArrayNode));
    if (ArrayNode == NULL) {
        fprintf(stderr, "ArrayNode allocation failed.\n");
        return NULL;
    }

    ArrayNode->item = item;
    ArrayNode->next = NULL;

    struct Array *instance = (struct Array *) self;

    mutex_lock(&mutex);
    struct ArrayNode **cursor;
    for (cursor = &instance->list; *cursor; cursor = &(*cursor)->next) {}
    *cursor = ArrayNode; // ➕ Append new ArrayNode
    mutex_unlock(&mutex);

    return ArrayNode->item;
}

// 🔎 Check if ArrayNode contains item with read-lock
static bool containsValue(const struct IArray *self, const void *item) {
    mutex_lock_shared(&mutex);

    const struct Array *instance = (struct Array *) self;

    bool found = false;
    for (const struct ArrayNode *cursor = instance->list; cursor; cursor = cursor->next) {
        if (cursor->item == item) {
            found = true; // ✅ Found matching item
            break;
        }
    }

    mutex_unlock(&mutex);
    return found;
}

// ❌ Remove and return first item (shift) with write-lock
static const void *shift(struct IArray *self) {
    mutex_lock(&mutex);

    struct Array *instance = (struct Array *)self;
    struct ArrayNode *ArrayNode = instance->list;
    const void *item = NULL;
    if (ArrayNode) {
        instance->list = ArrayNode->next;    // 🔗 Update head
        item = ArrayNode->item;              // 🎯 Capture item
        free(ArrayNode);                     // 🧹 Free removed ArrayNode
    }

    mutex_unlock(&mutex);
    return item;
}

// ❌ Remove and return last item (pop) with write-lock
static const void *pop(struct IArray *self) {
    mutex_lock(&mutex);

    struct Array *instance = (struct Array *)self;
    struct ArrayNode **cursor;
    for (cursor = &instance->list; *cursor && (*cursor)->next; cursor = &(*cursor)->next) {}

    const void *item = NULL;
    if (*cursor) {
        struct ArrayNode *ArrayNode = *cursor;
        item = ArrayNode->item;  // 🎯 Capture item
        *cursor = NULL;     // 🔗 Remove last ArrayNode
        free(ArrayNode);         // 🧹 Free ArrayNode
    }

    mutex_unlock(&mutex);
    return item;
}

// ❌ Remove first matching item, return item pointer with write-lock
static void *removeItem(struct IArray *self, const void *item) {
    mutex_lock(&mutex);

    struct Array *instance = (struct Array *)self;
    void *data = NULL;
    for (struct ArrayNode **cursor = &instance->list; *cursor; cursor = &(*cursor)->next) {
        if ((*cursor)->item == item) {
            struct ArrayNode *ArrayNode = *cursor;
            *cursor = (*cursor)->next; // 🔗 Remove ArrayNode from ArrayNode

            data = (void *)ArrayNode->item;  // 🎯 Return item
            free(ArrayNode);                 // 🧹 Free ArrayNode
            break;
        }
    }

    mutex_unlock(&mutex);
    return data;
}

// 🆚 Clone the ArrayNode (shallow copy) with read-lock
static struct IArray *clone(const struct IArray *self) {

    mutex_lock_shared(&mutex); // 🔒 Acquire read lock

    const struct Array *instance = (struct Array *)self;
    struct ArrayNode *copy = NULL;
    for (struct ArrayNode *cursor = instance->list, **copyPtr = &copy; cursor; cursor = cursor->next) {
        struct ArrayNode *ArrayNode = malloc(sizeof(struct ArrayNode));
        if (!ArrayNode) {
            mutex_unlock(&mutex);
            fprintf(stderr, "ArrayNode allocation failed.\n");
            return NULL;
        }

        ArrayNode->item = cursor->item;  // 🆕 Shallow copy item pointer
        ArrayNode->next = NULL;
        *copyPtr = ArrayNode;            // 🔗 Append ArrayNode to new ArrayNode
        copyPtr = &ArrayNode->next;
    }

    mutex_unlock(&mutex);
    struct IArray *arr = collection_array_new();
    ((struct Array *)arr)->list = copy; // 🔄 Set cloned ArrayNode head
    return arr;
}

// 🔢 Count number of items with read-lock
static int size(const struct IArray *self) {
    mutex_lock_shared(&mutex);

    int count = 0;
    for (const struct ArrayNode *cursor = ((struct Array *)self)->list; cursor; cursor = cursor->next, count++) {}

    mutex_unlock(&mutex);
    return count;
}

// 🧹 Clear the ArrayNode, optional callback for each item, with write-lock
static void clear(struct IArray *self, void (*callback)(void *item)) {
    mutex_lock(&mutex);

    for (struct ArrayNode **cursor = &((struct Array *)self)->list; *cursor;) {
        struct ArrayNode *ArrayNode = *cursor;
        *cursor = (*cursor)->next;         // 🔗 Remove ArrayNode from ArrayNode
        if (callback) callback((void *) ArrayNode->item);  // 🔔 User cleanup callback
        free(ArrayNode);                        // 🧹 Free ArrayNode memory
    }

    mutex_unlock(&mutex);
}

// 🔧 Initialize array base function pointers
struct Array *collection_array_init(struct Array *array) {
    if (array == NULL) return NULL;
    mutex_init(&mutex);
    array->list = NULL;
    array->base.unshift = unshift;
    array->base.push = push;
    array->base.containsValue = containsValue;
    array->base.shift = shift;
    array->base.pop = pop;
    array->base.removeItem = removeItem;
    array->base.clone = clone;
    array->base.size = size;
    array->base.clear = clear;
    return array;
}

// 🆕 Allocate new Array instance
struct Array *collection_array_alloc() {
    struct Array *array = malloc(sizeof(struct Array));
    if (array == NULL) {
        fprintf(stderr, "Array allocation failed.\n");
        return NULL;
    }
    memset(array, 0, sizeof(struct Array));
    return array;
}

// 🆕 Create new IArray instance
struct IArray *collection_array_new() {
    return (struct IArray *)collection_array_init(collection_array_alloc());
}

// 🧹 Free Array instance (note: ArrayNode should be cleared before free)
void collection_array_free(struct IArray **array) {
    mutex_destroy(&mutex);
    free(*array);
    *array = NULL;
}
