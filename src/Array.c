/**
* @file Array.c
* @internal
* @brief Array Implementation
*
* @author Saad Shams https://linkedin.com/in/muizz
* @copyright BSD 3-Clause License
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Array.h"

// 📌 Access element at given index, or NULL if out of range
static const void *get(const struct IArray *self, const int n) {
    if (self == NULL || n < 0) return NULL;

    struct Array *this = (struct Array *) self;
    mutex_lock_shared(&this->mutex);

    int index = 0;
    for (const struct ArrayNode *cursor = this->list; cursor; cursor = cursor->next, index++) {
        if (index == n) {
            mutex_unlock(&this->mutex);
            return cursor->item;
        }
    }

    mutex_unlock(&this->mutex);
    return NULL;
}

// 🔄 Iteration calling callback on each element with its index
static void forEach(const struct IArray *self, void (*callback)(const void *element, const void *data, const char **error), const void *data, const char **error) {
    struct Array *this = (struct Array *) self;
    mutex_lock_shared(&this->mutex);

    for (const struct ArrayNode *cursor = this->list; cursor; cursor = cursor->next) {
        callback(cursor->item, data, error);
    }

    mutex_unlock(&this->mutex);
}

// 🔍 Search returning the first element matching the predicate
static const void *find(const struct IArray *self, bool (*predicate)(const void *element, const void *data), const void *data) {
    struct Array *this = (struct Array *) self;
    mutex_lock_shared(&this->mutex);

    for (const struct ArrayNode *cursor = this->list; cursor; cursor = cursor->next) {
        if (predicate(cursor->item, data)) {
            mutex_unlock(&this->mutex);
            return cursor->item;
        }
    }

    mutex_unlock(&this->mutex);
    return NULL;
}

// ➕ Insert item at head (unshift)
static const void *unshift(struct IArray *self, const void *item, const char **error) {
    struct Array *this = (struct Array *) self;

    struct ArrayNode *node = malloc(sizeof(struct ArrayNode));
    if (node == NULL) return *error = "[Collection::Array::unshift] Error: Failed to allocate ArrayNode.", NULL;

    mutex_lock(&this->mutex);

    node->item = item;              // 🆕 Store item
    node->next = this->list;    // 🔗 Link old head
    this->list = node;          // 🔄 Update head pointer

    mutex_unlock(&this->mutex);
    return item;
}

// ➕ Append item at tail (push)
static const void *push(struct IArray *self, const void *item, const char **error) {
    struct Array *this = (struct Array *) self;

    struct ArrayNode *node = malloc(sizeof(struct ArrayNode));
    if (node == NULL) return *error = "[Collection::Array::push] Error: Failed to allocate ArrayNode.", NULL;

    node->item = item;
    node->next = NULL;

    mutex_lock(&this->mutex);

    struct ArrayNode **cursor;
    for (cursor = &this->list; *cursor; cursor = &(*cursor)->next) {}
    *cursor = node; // ➕ Append new ArrayNode

    mutex_unlock(&this->mutex);
    return node->item;
}

// 🔎 Check if ArrayNode contains item
static bool containsValue(const struct IArray *self, const void *item) {
    struct Array *this = (struct Array *) self;
    mutex_lock_shared(&this->mutex);

    bool found = false;
    for (const struct ArrayNode *cursor = this->list; cursor; cursor = cursor->next) {
        if (cursor->item == item) {
            found = true; // ✅ Found matching item
            break;
        }
    }

    mutex_unlock(&this->mutex);
    return found;
}

// ❌ Remove and return first item (shift)
static const void *shift(struct IArray *self) {
    struct Array *this = (struct Array *) self;
    mutex_lock(&this->mutex);

    struct ArrayNode *node = this->list;
    const void *item = NULL;
    if (node) {
        this->list = node->next;    // 🔗 Update head
        item = node->item;              // 🎯 Capture item
        free(node);                     // 🧹 Free removed ArrayNode
    }

    mutex_unlock(&this->mutex);
    return item;
}

// ❌ Remove and return last item (pop)
static const void *pop(struct IArray *self) {
    struct Array *this = (struct Array *) self;
    mutex_lock(&this->mutex);

    struct ArrayNode **cursor;
    for (cursor = &this->list; *cursor && (*cursor)->next; cursor = &(*cursor)->next) {}

    const void *item = NULL;
    if (*cursor) {
        struct ArrayNode *node = *cursor;
        item = node->item;  // 🎯 Capture item
        *cursor = NULL;     // 🔗 Remove last ArrayNode
        free(node);         // 🧹 Free ArrayNode
    }

    mutex_unlock(&this->mutex);
    return item;
}

// ❌ Remove first matching item, return item pointer
static void *removeItem(struct IArray *self, const void *item) {
    struct Array *this = (struct Array *) self;
    mutex_lock(&this->mutex);

    void *data = NULL;
    for (struct ArrayNode **cursor = &this->list; *cursor; cursor = &(*cursor)->next) {
        if ((*cursor)->item == item) {
            struct ArrayNode *node = *cursor;
            *cursor = (*cursor)->next; // 🔗 Remove ArrayNode from ArrayNode

            data = (void *)node->item;  // 🎯 Return item
            free(node);                 // 🧹 Free ArrayNode
            break;
        }
    }

    mutex_unlock(&this->mutex);
    return data;
}

// 🆚 Clone the ArrayNode (shallow copy)
static struct IArray *clone(const struct IArray *self, const char **error) {
    struct Array *this = (struct Array *) self;
    mutex_lock_shared(&this->mutex); // 🔒 Acquire read lock

    struct ArrayNode *copy = NULL;
    for (struct ArrayNode *cursor = this->list, **copyPtr = &copy; cursor; cursor = cursor->next) {
        struct ArrayNode *node = malloc(sizeof(struct ArrayNode));
        if (node == NULL) {
            mutex_unlock(&this->mutex);
            while (copy) {
                struct ArrayNode *temp = copy;
                copy = copy->next;
                free(temp);
            }
            return *error = "[Collection::Array::clone] Error: Failed to allocate ArrayNode.", NULL;
        }

        node->item = cursor->item;  // 🆕 Shallow copy item pointer
        node->next = NULL;
        *copyPtr = node;            // 🔗 Append ArrayNode to new ArrayNode
        copyPtr = &node->next;
    }

    mutex_unlock(&this->mutex);

    struct IArray *arr = collection_array_new(error);
    if (*error != NULL) { // cleanup
        for (struct ArrayNode **cursor = &copy; *cursor;) {
            struct ArrayNode *temp = *cursor;
            *cursor = (*cursor)->next;
            free(temp);
        }
        return NULL;
    }

    ((struct Array *) arr)->list = copy; // 🔄 Set cloned ArrayNode head
    return arr;
}

// 🔢 Count number of items with read-lock
static int size(const struct IArray *self) {
    struct Array *this = (struct Array *) self;
    mutex_lock_shared(&this->mutex);

    int count = 0;
    for (const struct ArrayNode *cursor = ((struct Array *) self)->list; cursor; cursor = cursor->next, count++) {}

    mutex_unlock(&this->mutex);
    return count;
}

// 🧹 Clear the ArrayNode, optional callback for each item
static void clear(struct IArray *self, void (*callback)(void *item)) {
    struct Array *this = (struct Array *) self;
    mutex_lock(&this->mutex);

    for (struct ArrayNode **cursor = &((struct Array *) self)->list; *cursor;) {
        struct ArrayNode *node = *cursor;
        *cursor = (*cursor)->next;         // 🔗 Remove ArrayNode from ArrayNode
        if (callback) callback((void *) node->item);  // 🔔 User cleanup callback
        free(node);                        // 🧹 Free ArrayNode memory
    }

    mutex_unlock(&this->mutex);
}

// 🔧 Initialize array base function pointers
static struct Array *init(struct Array *array) {
    if (array == NULL) return NULL;
    mutex_init(&array->mutex);
    array->list = NULL;
    array->base.get = get;
    array->base.forEach = forEach;
    array->base.find = find;
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
static struct Array *alloc(const char **error) {
    struct Array *array = malloc(sizeof(struct Array));
    if (array == NULL) return *error = "[Collection::Array::alloc] Error: Failed to allocate Array.", NULL;

    memset(array, 0, sizeof(struct Array));
    return array;
}

// 🆕 Create new IArray instance
struct IArray *collection_array_new(const char **error) {
    return (struct IArray *) init(alloc(error));
}

// 🧹 Free Array instance (note: ArrayNode should be cleared before free)
void collection_array_free(struct IArray **array) {
    if (array == NULL || *array == NULL) return;

    struct Array *this = (struct Array *) *array;
    mutex_destroy(&this->mutex);
    free(*array);

    *array = NULL;
}
