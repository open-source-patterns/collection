/**
* @file Dictionary.c
* @internal
* @brief Dictionary Implementation
*
* @author Saad Shams https://linkedin.com/in/muizz
* @copyright BSD 3-Clause License
*/
#include <stdlib.h>
#include <string.h>

#include "Dictionary.h"

#define INITIAL_CAPACITY 16

// 🔑 djb2 string hash function
static unsigned long hash(const char *str) { // djb2 hash
    unsigned long hash = 5381;
    while (*str) {
        hash = (hash << 5) + hash + (unsigned char)*str++;
    }
    return hash;
}

// 🔍 Retrieve value by key with read-lock
static const void *get(const struct IDictionary *self, const char *key) {
    struct Dictionary *this = (struct Dictionary *) self;
    mutex_lock_shared(&this->mutex);

    const unsigned long index = hash(key) % this->capacity;

    const void *value = NULL;
    for (const struct DictionaryNode *cursor = this->buckets[index]; cursor; cursor = cursor->next) {
        if (strcmp(cursor->key, key) == 0) {
            value = cursor->value; // 🎯 Found value
            break;
        }
    }

    mutex_unlock(&this->mutex);
    return value;
}

// ➕ Insert key-value pair with write-lock
static bool put(struct IDictionary *self, const char *key, const void *value, const char **error) {
    struct Dictionary *this = (struct Dictionary *) self;
    mutex_lock(&this->mutex);

    const unsigned long index = hash(key) % this->capacity;

    struct DictionaryNode *node = malloc(sizeof(struct DictionaryNode));
    if (node == NULL) return *error = "[Collection::Dictionary::put] Error: Failed to allocate DictionaryNode.", NULL;

    node->key = strdup(key); // 🆕 Duplicate key string for ownership
    if (node->key == NULL) return *error = "[Collection::Dictionary::put] Error: Failed to allocate DictionaryNode key.", NULL;
    node->value = value;
    node->next = NULL;

    // 🔗 Append new DictionaryNode to bucket's linked list
    struct DictionaryNode **cursor;
    for(cursor = &this->buckets[index]; *cursor; cursor = &(*cursor)->next) {}
    *cursor = node;

    mutex_unlock(&this->mutex);
    return true;
}

// 🔎 Check existence of key with read-lock
static bool containsKey(const struct IDictionary *self, const char *key) {
    struct Dictionary *this = (struct Dictionary *) self;
    mutex_lock_shared(&this->mutex);

    const unsigned long index = hash(key) % this->capacity;

    bool found = false;
    for (const struct DictionaryNode *cursor = this->buckets[index]; cursor; cursor = cursor->next) {
        if (strcmp(cursor->key, key) == 0) {
            found = true; // ✅ Key found
            break;
        }
    }

    mutex_unlock(&this->mutex);
    return found;
}

// Remove key and return associated value with write-lock
void *removeItem(struct IDictionary *self, const char *key) {
    struct Dictionary *this = (struct Dictionary *) self;
    mutex_lock(&this->mutex);

    const unsigned long index = hash(key) % this->capacity;
    void *value = NULL;

    for (struct DictionaryNode **cursor = &this->buckets[index]; *cursor; cursor = &(*cursor)->next) {
        struct DictionaryNode *node = *cursor;
        if (strcmp(node->key, key) == 0) {
            *cursor = node->next;       // 🔗 Remove DictionaryNode from list
            value = (void *)node->value;

            free((void *) node->key);   // 🧹 Free duplicated key
            free(node);                 // 🧹 Free DictionaryNode
            break;
        }
    }

    mutex_unlock(&this->mutex);
    return value; // 🔍 Not found
}

// 🔄 Replace value of existing key with write-lock
static void *replace(const struct IDictionary *self, const char *key, const void *value) {
    struct Dictionary *this = (struct Dictionary *) self;
    mutex_lock(&this->mutex);

    const unsigned long index = hash(key) % this->capacity;
    void *oldValue = NULL;

    // 🔍 Traverse the bucket's linked list
    for (struct DictionaryNode *cursor = this->buckets[index]; cursor; cursor = cursor->next) {
        if (strcmp(cursor->key, key) == 0) {
            oldValue = (void *)cursor->value;
            cursor->value = value; // 🔄 Update value

            break;
        }
    }

    mutex_unlock(&this->mutex);
    return oldValue; // 🔍 Not found
}

// 🧹 Clear all entries with write-lock, optional callback on each value
static void clear(const struct IDictionary *self, void (*callback)(void *value)) {
    struct Dictionary *this = (struct Dictionary *) self;
    mutex_lock(&this->mutex);

    for (size_t i = 0; i < this->capacity; ++i) {
        struct DictionaryNode *current = this->buckets[i];
        while (current != NULL) {
            struct DictionaryNode *node = current;
            current = current->next;
            free((void *) node->key); // 🧹 Free key
            if (callback) callback((void *) node->value); // 🔔 User cleanup
            free(node); // 🧹 Free DictionaryNode
        }
        this->buckets[i] = NULL; // 🔄 Reset bucket pointer
    }
    free(this->buckets); // 🧹 Free DictionaryNode array
    this->buckets = NULL;

    mutex_unlock(&this->mutex);
}

// 🔧 Initialize dictionary base functions
static struct Dictionary *init(struct Dictionary *dictionary) {
    if (dictionary == NULL) return NULL;
    mutex_init(&dictionary->mutex);
    dictionary->base.get = get;
    dictionary->base.put = put;
    dictionary->base.containsKey = containsKey;
    dictionary->base.removeItem = removeItem;
    dictionary->base.replace = replace;
    dictionary->base.clear = clear;
    return dictionary;
}

// 🆕 Allocate and initialize new dictionary
static struct Dictionary *alloc(const char **error) {
    struct Dictionary *dictionary = malloc(sizeof(struct Dictionary));
    if (dictionary == NULL) return *error = "[Collection::Dictionary::alloc] Error: Failed to allocate Dictionary.", NULL;

    memset(dictionary, 0, sizeof(struct Dictionary));

    dictionary->capacity = INITIAL_CAPACITY;
    dictionary->size = 0;
    dictionary->buckets = calloc(dictionary->capacity, sizeof(struct DictionaryNode *));
    return dictionary;
}

// 🆕 Create new IDictionary instance
struct IDictionary *collection_dictionary_new(const char **error) {
    return (struct IDictionary *) init(alloc(error));
}

// 🧹 Free dictionary instance (note: DictionaryNode entries already freed by clear)
void collection_dictionary_free(struct IDictionary **dictionary) {
    if (dictionary == NULL || *dictionary == NULL) return;

    struct Dictionary *this = (struct Dictionary *) *dictionary;
    mutex_destroy(&this->mutex);
    free(*dictionary);

    *dictionary = NULL;
}
