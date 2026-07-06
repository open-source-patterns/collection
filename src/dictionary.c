/**
* @file dictionary.c
* @internal
* @brief Dictionary Implementation
*
* @author Saad Shams https://linkedin.com/in/muizz
* @copyright BSD 3-Clause License
*/
#include "dictionary.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#define strdup _strdup
#endif

#pragma region Constants

#define INITIAL_CAPACITY 16

#pragma endregion

#pragma region Helpers

// Computes the hash value for a string key.
static unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    while (*str) {
        hash = (hash << 5) + hash + (unsigned char)*str++;
    }
    return hash;
}

#pragma endregion

#pragma region Operations

// Returns the value associated with the specified key.
static void *get(const struct IDictionary *self, const char *key) {
    struct Dictionary *this = (struct Dictionary *) self;

    mutex_lock_shared(&this->mutex);

    const unsigned long index = hash(key) % this->capacity;
    void *value = NULL;

    for (const struct DictionaryNode *cursor = this->buckets[index]; cursor; cursor = cursor->next) {
        if (strcmp(cursor->key, key) == 0) {
            value = (void *) cursor->value; // Found value
            break;
        }
    }

    mutex_unlock(&this->mutex);
    return value;
}

// Inserts a new key-value pair.
static bool put(struct IDictionary *self, const char *key, const void *value) {
    struct Dictionary *this = (struct Dictionary *) self;
    bool added = false;

    mutex_lock(&this->mutex);

    const unsigned long index = hash(key) % this->capacity;
    struct DictionaryNode *node = malloc(sizeof(struct DictionaryNode));
    if (node == NULL) {
        fprintf(stderr, "\033[0;31m[Collection::Dictionary::put] Error: Failed to allocate DictionaryNode.\033[0m\n");
        goto out_unlock;
    }

    node->key = strdup(key); // Own a copy of the key because the caller may release or mutate its string.
    if (node->key == NULL) {
        fprintf(stderr, "\033[0;31m[Collection::Dictionary::put] Error: Failed to allocate DictionaryNode key.\033[0m\n");
        free(node);
        goto out_unlock;
    }
    node->value = value;
    node->next = NULL;
    this->size++;

    // Append new DictionaryNode to bucket's linked list
    struct DictionaryNode **cursor;
    for(cursor = &this->buckets[index]; *cursor; cursor = &(*cursor)->next) {}
    *cursor = node;
    added = true;

out_unlock:
    mutex_unlock(&this->mutex);
    return added;
}

// Returns whether the specified key exists.
static bool contains_key(const struct IDictionary *self, const char *key) {
    struct Dictionary *this = (struct Dictionary *) self;
    bool found = false;

    mutex_lock_shared(&this->mutex);

    const unsigned long index = hash(key) % this->capacity;
    for (const struct DictionaryNode *cursor = this->buckets[index]; cursor; cursor = cursor->next) {
        if (strcmp(cursor->key, key) == 0) {
            found = true;
            break;
        }
    }

    mutex_unlock(&this->mutex);
    return found;
}

// Removes the specified key-value pair.
void *remove_item(struct IDictionary *self, const char *key) {
    struct Dictionary *this = (struct Dictionary *) self;
    void *value = NULL;

    mutex_lock(&this->mutex);

    const unsigned long index = hash(key) % this->capacity;
    for (struct DictionaryNode **cursor = &this->buckets[index]; *cursor; cursor = &(*cursor)->next) {
        struct DictionaryNode *node = *cursor;
        if (strcmp(node->key, key) == 0) {
            *cursor = node->next;       // Remove DictionaryNode
            value = (void *) node->value;

            free((void *) node->key);
            free(node);
            this->size--;
            break;
        }
    }

    mutex_unlock(&this->mutex);
    return value;
}

// Replaces the value associated with the specified key.
static void *replace(const struct IDictionary *self, const char *key, const void *value) {
    struct Dictionary *this = (struct Dictionary *) self;
    void *temp = NULL;

    mutex_lock(&this->mutex);

    const unsigned long index = hash(key) % this->capacity;
    for (struct DictionaryNode *cursor = this->buckets[index]; cursor; cursor = cursor->next) {
        if (strcmp(cursor->key, key) == 0) {
            temp = (void *) cursor->value;
            cursor->value = value;
            break;
        }
    }

    mutex_unlock(&this->mutex);
    return temp;
}

// Removes all key-value pairs from the dictionary.
static bool clear(const struct IDictionary *self, void (*destructor)(void *value)) {
    struct Dictionary *this = (struct Dictionary *) self;
    mutex_lock(&this->mutex);

    for (size_t i = 0; i < this->capacity; ++i) {
        struct DictionaryNode *current = this->buckets[i];
        while (current != NULL) {
            struct DictionaryNode *node = current;
            current = current->next;
            free((void *) node->key);
            if (destructor) destructor((void *) node->value);
            free(node);
        }
        this->buckets[i] = NULL; // Reset bucket pointer
    }
    this->size = 0;

    mutex_unlock(&this->mutex);
    return true;
}

#pragma endregion

#pragma region Memory Management

// Returns the aligned allocation size for Dictionary.
static size_t size(void) {
    return (sizeof(struct Dictionary) + (sizeof(void *) - 1u)) & ~(sizeof(void *) - 1u);
}

// Allocates a Dictionary instance.
static struct IDictionary *alloc() {
    struct IDictionary *dictionary = malloc(size());

    if (dictionary == NULL) {
        fprintf(stderr, "\033[0;31m[Collection::Dictionary::alloc] ERROR: Instance allocation failed.\033[0m\n");
        return NULL;
    }

    return dictionary;
}

// Initializes a Dictionary instance.
static struct IDictionary *init(struct IDictionary *dictionary) {
    if (dictionary == NULL) return NULL;

    struct Dictionary *this = (struct Dictionary *) dictionary;
    memset(this, 0, sizeof(struct Dictionary));

    this->capacity = INITIAL_CAPACITY;
    this->size = 0;
    this->buckets = calloc(this->capacity, sizeof(struct DictionaryNode *));
    if (this->buckets == NULL) goto exception;

    if (mutex_init(&this->mutex) != 0) goto exception;

    this->super.get = get;
    this->super.put = put;
    this->super.contains_key = contains_key;
    this->super.remove_item = remove_item;
    this->super.replace = replace;
    this->super.clear = clear;

    return dictionary;

exception:
    collection_dictionary_dealloc(&dictionary, NULL);
    return NULL;
}

#pragma endregion

#pragma region Public API

// Creates a new dictionary instance.
struct IDictionary *collection_dictionary_new(void) {
    return init(alloc());
}

// Destroys a dictionary instance. (Optional destructor to free entries)
void collection_dictionary_dealloc(struct IDictionary **dictionary, void (*destructor)(void *item)) {
    if (dictionary == NULL) return;
    struct Dictionary *this = (struct Dictionary *) *dictionary;

    (*dictionary)->clear(*dictionary, destructor);

    free(this->buckets);
    this->buckets = NULL;
    this->capacity = 0;

    mutex_destroy(&this->mutex);

    free(*dictionary);
    *dictionary = NULL;
}

#pragma endregion
