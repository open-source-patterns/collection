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

#pragma region Query Operations

// Returns the element at the specified index, or NULL if out of range.
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


// Invokes a callback for each element.
static void for_each(const struct IArray *self, void (*consumer)(const void *element, const void *data), const void *data) {
    struct Array *this = (struct Array *) self;
    mutex_lock_shared(&this->mutex);

    for (const struct ArrayNode *cursor = this->list; cursor; cursor = cursor->next) {
        consumer(cursor->item, data);
    }

    mutex_unlock(&this->mutex);
}

// Finds the first element matching a predicate.
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

// Returns the index of the first matching element.
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
    return (size_t) - 1; // No matching element found.
}

// Returns the index of the last matching element.
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


// Returns whether the array contains the specified element.
static bool contains_value(const struct IArray *self, const void *item) {
    struct Array *this = (struct Array *) self;
    mutex_lock_shared(&this->mutex);

    bool found = false;
    for (const struct ArrayNode *cursor = this->list; cursor; cursor = cursor->next) {
        if (cursor->item == item) {
            found = true;
            break;
        }
    }

    mutex_unlock(&this->mutex);
    return found;
}

// Returns the number of elements.
static size_t count(const struct IArray *self) {
    struct Array *this = (struct Array *) self;
    mutex_lock_shared(&this->mutex);

    size_t count = 0;
    for (const struct ArrayNode *cursor = ((struct Array *) self)->list; cursor; cursor = cursor->next, count++) {}

    mutex_unlock(&this->mutex);
    return count;
}

// Creates a shallow copy of the array.
static struct IArray *array_clone(const struct IArray *self) {
    struct Array *this = (struct Array *) self;
    mutex_lock_shared(&this->mutex); // Acquire read lock

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

        node->item = cursor->item;  // Shallow copy item pointer
        node->next = NULL;
        *copyPtr = node;            // Append node to new list
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

    // Transfer ownership of the cloned node chain.
    ((struct Array *) arr)->list = copy;
    return arr;
}

#pragma endregion

#pragma region Mutation Operations

// Replaces the element at the specified index.
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

// Inserts an element at the beginning of the array.
static const void *unshift(struct IArray *self, const void *item) {
    struct Array *this = (struct Array *) self;

    struct ArrayNode *node = malloc(sizeof(struct ArrayNode));
    if (node == NULL) {
        fprintf(stderr, "\033[0;31m[Collection::Array::unshift] Error: Failed to allocate ArrayNode.\033[0m\n");
        return NULL;
    }

    mutex_lock(&this->mutex);

    node->item = item;          // Store item
    node->next = this->list;    // Link old head
    this->list = node;          // Update head pointer

    mutex_unlock(&this->mutex);
    return item;
}

// Appends an element to the end of the array.
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
    *cursor = node; // Append new ArrayNode
    success = true;

    mutex_unlock(&this->mutex);
    return success;
}


// Removes and returns the first element.
static const void *shift(struct IArray *self) {
    struct Array *this = (struct Array *) self;
    mutex_lock(&this->mutex);

    struct ArrayNode *node = this->list;
    const void *item = NULL;
    if (node) {
        this->list = node->next;    // Update head
        item = node->item;          // Capture item
        free(node);                 // Free removed ArrayNode
    }

    mutex_unlock(&this->mutex);
    return item;
}

// Removes and returns the last element.
static const void *pop(struct IArray *self) {
    struct Array *this = (struct Array *) self;
    mutex_lock(&this->mutex);

    struct ArrayNode **cursor;
    for (cursor = &this->list; *cursor && (*cursor)->next; cursor = &(*cursor)->next) {}

    const void *item = NULL;
    if (*cursor) {
        struct ArrayNode *node = *cursor;
        item = node->item;  // Capture item
        *cursor = NULL;     // Remove last ArrayNode
        free(node);         // Free ArrayNode
    }

    mutex_unlock(&this->mutex);
    return item;
}

// Removes the specified element.
static void *remove_item(struct IArray *self, const void *item) {
    struct Array *this = (struct Array *) self;
    mutex_lock(&this->mutex);

    void *data = NULL;
    for (struct ArrayNode **cursor = &this->list; *cursor; cursor = &(*cursor)->next) {
        if ((*cursor)->item == item) {
            struct ArrayNode *node = *cursor;
            *cursor = (*cursor)->next;    // Remove ArrayNode from ArrayNode
            data = (void *) node->item;   // Return item
            free(node);                   // Free ArrayNode
            break;
        }
    }

    mutex_unlock(&this->mutex);
    return data;
}

// Removes all elements from the array.
static void clear(struct IArray *self, void (*destructor)(void *item)) {
    struct Array *this = (struct Array *) self;
    mutex_lock(&this->mutex);

    for (struct ArrayNode **cursor = &((struct Array *) self)->list; *cursor;) {
        struct ArrayNode *node = *cursor;
        *cursor = (*cursor)->next;         // Remove ArrayNode from ArrayNode
        if (destructor) destructor((void *) node->item);
        free(node);
    }

    mutex_unlock(&this->mutex);
}

#pragma endregion

#pragma region Memory Management

// Returns the aligned allocation size for Array.
static size_t size(void) {
    return (sizeof(struct Array) + (sizeof(void *) - 1u)) & ~(sizeof(void *) - 1u); // Align to pointer size.
}

// Allocates an Array instance.
static struct IArray *alloc() {
    struct IArray *array = malloc(size());

    if (array == NULL) {
        fprintf(stderr, "\033[0;31m[Collection::Array::alloc] ERROR: Instance allocation failed.\033[0m\n");
        return NULL;
    }

    return array;
}

// Initializes an Array instance.
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

#pragma endregion

#pragma region Public API

// Creates a new array instance.
struct IArray *collection_array_new(void) {
    return init(alloc());
}

// Destroys an array instance.
void collection_array_dealloc(struct IArray **array, void (*destructor)(void *item)) {
    if (array == NULL) return;
    struct Array *this = (struct Array *) *array;

    (*array)->clear(*array, destructor);
    mutex_destroy(&this->mutex);

    free(*array);
    *array = NULL;
}

#pragma endregion
