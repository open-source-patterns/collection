#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Array.h"

// 📌 Access element at given index, or NULL if out of range
static const void *get(const struct IArray *self, const int n) {
    if (self == NULL || n < 0) return NULL;

    struct Array *this = (struct Array *)self;
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
static void forEach(const struct IArray *self, void (*callback)(const void *element, int index, const void *data), const void *data) {
    struct Array *this = (struct Array *)self;
    mutex_lock_shared(&this->mutex);

    int index = 0;
    for (const struct ArrayNode *cursor = this->list; cursor; cursor = cursor->next, index++) {
        callback(cursor->item, index, data);
    }

    mutex_unlock(&this->mutex);
}

// 🔍 Search returning the first element matching the predicate
static const void *find(const struct IArray *self, bool (*predicate)(const void *element, const void *data), const void *data) {
    struct Array *this = (struct Array *)self;
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
static const void *unshift(struct IArray *self, const void *item) {
    struct Array *this = (struct Array *)self;
    struct ArrayNode *ArrayNode = malloc(sizeof(struct ArrayNode));
    if (ArrayNode == NULL) {
        fprintf(stderr, "ArrayNode allocation failed.\n");
        return NULL;
    }

    mutex_lock(&this->mutex);

    ArrayNode->item = item;              // 🆕 Store item
    ArrayNode->next = this->list;    // 🔗 Link old head
    this->list = ArrayNode;          // 🔄 Update head pointer

    mutex_unlock(&this->mutex);
    return item;
}

// ➕ Append item at tail (push)
static const void *push(struct IArray *self, const void *item) {
    struct Array *this = (struct Array *)self;
    struct ArrayNode *ArrayNode = malloc(sizeof(struct ArrayNode));
    if (ArrayNode == NULL) {
        fprintf(stderr, "ArrayNode allocation failed.\n");
        return NULL;
    }

    ArrayNode->item = item;
    ArrayNode->next = NULL;

    mutex_lock(&this->mutex);

    struct ArrayNode **cursor;
    for (cursor = &this->list; *cursor; cursor = &(*cursor)->next) {}
    *cursor = ArrayNode; // ➕ Append new ArrayNode

    mutex_unlock(&this->mutex);
    return ArrayNode->item;
}

// 🔎 Check if ArrayNode contains item
static bool containsValue(const struct IArray *self, const void *item) {
    struct Array *this = (struct Array *)self;
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
    struct Array *this = (struct Array *)self;
    mutex_lock(&this->mutex);

    struct ArrayNode *ArrayNode = this->list;
    const void *item = NULL;
    if (ArrayNode) {
        this->list = ArrayNode->next;    // 🔗 Update head
        item = ArrayNode->item;              // 🎯 Capture item
        free(ArrayNode);                     // 🧹 Free removed ArrayNode
    }

    mutex_unlock(&this->mutex);
    return item;
}

// ❌ Remove and return last item (pop)
static const void *pop(struct IArray *self) {
    struct Array *this = (struct Array *)self;
    mutex_lock(&this->mutex);

    struct ArrayNode **cursor;
    for (cursor = &this->list; *cursor && (*cursor)->next; cursor = &(*cursor)->next) {}

    const void *item = NULL;
    if (*cursor) {
        struct ArrayNode *ArrayNode = *cursor;
        item = ArrayNode->item;  // 🎯 Capture item
        *cursor = NULL;     // 🔗 Remove last ArrayNode
        free(ArrayNode);         // 🧹 Free ArrayNode
    }

    mutex_unlock(&this->mutex);
    return item;
}

// ❌ Remove first matching item, return item pointer
static void *removeItem(struct IArray *self, const void *item) {
    struct Array *this = (struct Array *)self;
    mutex_lock(&this->mutex);

    void *data = NULL;
    for (struct ArrayNode **cursor = &this->list; *cursor; cursor = &(*cursor)->next) {
        if ((*cursor)->item == item) {
            struct ArrayNode *ArrayNode = *cursor;
            *cursor = (*cursor)->next; // 🔗 Remove ArrayNode from ArrayNode

            data = (void *)ArrayNode->item;  // 🎯 Return item
            free(ArrayNode);                 // 🧹 Free ArrayNode
            break;
        }
    }

    mutex_unlock(&this->mutex);
    return data;
}

// 🆚 Clone the ArrayNode (shallow copy)
static struct IArray *clone(const struct IArray *self) {
    struct Array *this = (struct Array *)self;
    mutex_lock_shared(&this->mutex); // 🔒 Acquire read lock

    struct ArrayNode *copy = NULL;
    for (struct ArrayNode *cursor = this->list, **copyPtr = &copy; cursor; cursor = cursor->next) {
        struct ArrayNode *ArrayNode = malloc(sizeof(struct ArrayNode));
        if (!ArrayNode) {
            mutex_unlock(&this->mutex);
            fprintf(stderr, "ArrayNode allocation failed.\n");
            return NULL;
        }

        ArrayNode->item = cursor->item;  // 🆕 Shallow copy item pointer
        ArrayNode->next = NULL;
        *copyPtr = ArrayNode;            // 🔗 Append ArrayNode to new ArrayNode
        copyPtr = &ArrayNode->next;
    }

    mutex_unlock(&this->mutex);
    struct IArray *arr = collection_array_new();
    ((struct Array *)arr)->list = copy; // 🔄 Set cloned ArrayNode head
    return arr;
}

// 🔢 Count number of items with read-lock
static int size(const struct IArray *self) {
    struct Array *this = (struct Array *)self;
    mutex_lock_shared(&this->mutex);

    int count = 0;
    for (const struct ArrayNode *cursor = ((struct Array *)self)->list; cursor; cursor = cursor->next, count++) {}

    mutex_unlock(&this->mutex);
    return count;
}

// 🧹 Clear the ArrayNode, optional callback for each item
static void clear(struct IArray *self, void (*callback)(void *item)) {
    struct Array *this = (struct Array *)self;
    mutex_lock(&this->mutex);

    for (struct ArrayNode **cursor = &((struct Array *)self)->list; *cursor;) {
        struct ArrayNode *ArrayNode = *cursor;
        *cursor = (*cursor)->next;         // 🔗 Remove ArrayNode from ArrayNode
        if (callback) callback((void *) ArrayNode->item);  // 🔔 User cleanup callback
        free(ArrayNode);                        // 🧹 Free ArrayNode memory
    }

    mutex_unlock(&this->mutex);
}

// 🔧 Initialize array base function pointers
static struct Array *collection_array_init(struct Array *array) {
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
static struct Array *collection_array_alloc() {
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
    if (array == NULL || *array == NULL) return;

    struct Array *this = (struct Array *)*array;
    mutex_destroy(&this->mutex);
    free(*array);

    *array = NULL;
}
