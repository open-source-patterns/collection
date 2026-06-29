/**
* @file Array.c
* @internal
* @brief Array Implementation
*
* @author Saad Shams https://linkedin.com/in/muizz
* @copyright BSD 3-Clause License
*/
#include "array.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// 📌 Access element at given index, or NULL if out of range
static const void *get(const struct IArray *self, const size_t index) {
    struct Array *this = (struct Array *) self;
    mutex_lock_shared(&this->mutex);

    size_t i = 0;
    for (const struct ArrayNode *cursor = this->list; cursor; cursor = cursor->next, i++) {
        if (i == index) {
            mutex_unlock(&this->mutex);
            return cursor->item;
        }
    }

    mutex_unlock(&this->mutex);
    return NULL;
}

static void *put(struct IArray *self, const void *item, const size_t index) {
    struct Array *this = (struct Array *) self;
    mutex_lock_shared(&this->mutex);

    size_t i = 0;
    for (struct ArrayNode *cursor = this->list; cursor; cursor = cursor->next, i++) {
        if (i == index) {
            void *temp = (void *) cursor->item;
            cursor->item = item;
            mutex_unlock(&this->mutex);
            return temp;
        }
    }

    mutex_unlock(&this->mutex);
    return NULL;
}

// 🔄 Iteration calling callback on each element with its index
static void for_each(const struct IArray *self, void (*consumer)(const void *element, const void *data), const void *data) {
    struct Array *this = (struct Array *) self;
    mutex_lock_shared(&this->mutex);

    for (const struct ArrayNode *cursor = this->list; cursor; cursor = cursor->next) {
        consumer(cursor->item, data);
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

static size_t first_index(const struct IArray *self, bool (*predicate)(const void *element, const void *data), const void *data) {
    struct Array *this = (struct Array *) self;
    mutex_lock_shared(&this->mutex);

    size_t index = 0;
    for (const struct ArrayNode *cursor = this->list; cursor; cursor = cursor->next, index++) {
        if (predicate(cursor->item, data)) {
            mutex_unlock(&this->mutex);
            return index;
        }
    }

    mutex_unlock(&this->mutex);
    return SIZE_MAX;
}

static size_t last_index(const struct IArray *self, bool (*predicate)(const void *element, const void *data), const void *data) {
    struct Array *this = (struct Array *) self;
    mutex_lock_shared(&this->mutex);

    size_t i = 0, index = (size_t) -1;
    for (const struct ArrayNode *cursor = this->list; cursor; cursor = cursor->next, i++) {
        if (predicate(cursor->item, data))
            index = i;
    }

    mutex_unlock(&this->mutex);
    return index;
}

// ➕ Insert item at head (unshift)
static const void *unshift(struct IArray *self, const void *item) {
    struct Array *this = (struct Array *) self;

    struct ArrayNode *node = malloc(sizeof(struct ArrayNode));
    if (node == NULL) {
        fprintf(stderr, "\033[0;31m[Collection::Array::unshift] Error: Failed to allocate ArrayNode.\033[0m\n");
        return NULL;
    }

    mutex_lock(&this->mutex);

    node->item = item;              // 🆕 Store item
    node->next = this->list;    // 🔗 Link old head
    this->list = node;          // 🔄 Update head pointer

    mutex_unlock(&this->mutex);
    return item;
}

// ➕ Append item at tail (push)
static bool push(struct IArray *self, const void *item) {
    struct Array *this = (struct Array *) self;
    bool success = false;

    struct ArrayNode *node = malloc(sizeof(struct ArrayNode));
    if (node == NULL) {
        fprintf(stderr, "\033[0;31m[Collection::Array::push] Error: Failed to allocate ArrayNode.\033[0m\n");
        return success;
    }

    node->item = item;
    node->next = NULL;

    mutex_lock(&this->mutex);

    struct ArrayNode **cursor;
    for (cursor = &this->list; *cursor; cursor = &(*cursor)->next) {}
    *cursor = node; // ➕ Append new ArrayNode
    success = true;

    mutex_unlock(&this->mutex);
    return success;
}

// 🔎 Check if ArrayNode contains item
static bool contains_value(const struct IArray *self, const void *item) {
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
static void *remove_item(struct IArray *self, const void *item) {
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
static struct IArray *array_clone(const struct IArray *self) {
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
            fprintf(stderr, "\033[0;31m[Collection::Array::clone] Error: Failed to allocate ArrayNode.\033[0m\n");
            return NULL;
        }

        node->item = cursor->item;  // 🆕 Shallow copy item pointer
        node->next = NULL;
        *copyPtr = node;            // 🔗 Append node to new list
        copyPtr = &node->next;
    }

    mutex_unlock(&this->mutex);

    struct IArray *arr = collection_array_new(); // Create a new array container
    if (arr == NULL) { // cleanup
        for (struct ArrayNode **cursor = &copy; *cursor;) {
            struct ArrayNode *temp = *cursor;
            *cursor = (*cursor)->next;
            free(temp);
        }
        return NULL;
    }

    ((struct Array *) arr)->list = copy; // 🔄 Attach the copied node chain to the new array
    return arr;
}

// 🔢 Count number of items with read-lock
static size_t count(const struct IArray *self) {
    struct Array *this = (struct Array *) self;
    mutex_lock_shared(&this->mutex);

    size_t count = 0;
    for (const struct ArrayNode *cursor = ((struct Array *) self)->list; cursor; cursor = cursor->next, count++) {}

    mutex_unlock(&this->mutex);
    return count;
}

// 🧹 Clear the ArrayNode, optional callback for each item
static void clear(struct IArray *self, void (*destructor)(void *item)) {
    struct Array *this = (struct Array *) self;
    mutex_lock(&this->mutex);

    for (struct ArrayNode **cursor = &((struct Array *) self)->list; *cursor;) {
        struct ArrayNode *node = *cursor;
        *cursor = (*cursor)->next;         // 🔗 Remove ArrayNode from ArrayNode
        if (destructor) destructor((void *) node->item);  // 🔔 User cleanup callback
        free(node);                        // 🧹 Free ArrayNode memory
    }

    mutex_unlock(&this->mutex);
}

static size_t size(void) {
    return (sizeof(struct Array) + (sizeof(void *) - 1u)) & ~(sizeof(void *) - 1u);
}

static struct IArray *alloc() {
    struct IArray *array = malloc(size());

    if (array == NULL) {
        fprintf(stderr, "\033[0;31m[Collection::Array::alloc] ERROR: Instance allocation failed.\033[0m\n");
        return NULL;
    }

    return array;
}

// 🔧 Initialize array base function pointers
static struct IArray *init(struct IArray *array) {
    if (array == NULL) return NULL;

    struct Array *this = (struct Array *) array;
    memset(this, 0, sizeof(struct Array));

    if (mutex_init(&this->mutex) != 0) goto exception;

    this->list = NULL;
    this->super.get = get;
    this->super.put = put;
    this->super.for_each = for_each;
    this->super.find = find;
    this->super.first_index = first_index;
    this->super.last_index = last_index;
    this->super.unshift = unshift;
    this->super.push = push;
    this->super.contains_value = contains_value;
    this->super.shift = shift;
    this->super.pop = pop;
    this->super.remove_item = remove_item;
    this->super.clone = array_clone;
    this->super.count = count;
    this->super.clear = clear;

    return array;

exception:
    collection_array_dealloc(&array, NULL);
    return NULL;
}

struct IArray *collection_array_new(void) {
    return init(alloc());
}

// 🧹 DeInit Array instance (note: Optional destructor to free entries)
void collection_array_dealloc(struct IArray **array, void (*destructor)(void *item)) {
    if (array == NULL) return;
    struct Array *this = (struct Array *) *array;

    (*array)->clear(*array, destructor);
    mutex_destroy(&this->mutex);

    free(*array);
    *array = NULL;
}
