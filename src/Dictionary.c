#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
static bool put(struct IDictionary *self, const char *key, const void *value) {
    struct Dictionary *this = (struct Dictionary *) self;
    mutex_lock(&this->mutex);

    const unsigned long index = hash(key) % this->capacity;

    struct DictionaryNode *DictionaryNode = malloc(sizeof(struct DictionaryNode));
    if (DictionaryNode == NULL) {
        fprintf(stderr, "DictionaryNode allocation failed.\n");
        return false;
    }

    DictionaryNode->key = strdup(key); // 🆕 Duplicate key string for ownership
    DictionaryNode->value = value;
    DictionaryNode->next = NULL;

    // 🔗 Append new DictionaryNode to bucket's linked list
    struct DictionaryNode **cursor;
    for(cursor = &this->buckets[index]; *cursor; cursor = &(*cursor)->next) {}
    *cursor = DictionaryNode;

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
        struct DictionaryNode *DictionaryNode = *cursor;
        if (strcmp(DictionaryNode->key, key) == 0) {
            *cursor = DictionaryNode->next;       // 🔗 Remove DictionaryNode from list
            value = (void *)DictionaryNode->value;

            free((void *) DictionaryNode->key);   // 🧹 Free duplicated key
            free(DictionaryNode);                 // 🧹 Free DictionaryNode
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
            struct DictionaryNode *DictionaryNode = current;
            current = current->next;
            free((void *) DictionaryNode->key); // 🧹 Free key
            if (callback) callback((void *) DictionaryNode->value); // 🔔 User cleanup
            free(DictionaryNode); // 🧹 Free DictionaryNode
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
static struct Dictionary *alloc() {
    struct Dictionary *dictionary = malloc(sizeof(struct Dictionary));
    if (dictionary == NULL) {
        fprintf(stderr, "Dictionary allocation failed.\n");
        return NULL;
    }

    memset(dictionary, 0, sizeof(struct Dictionary));

    dictionary->capacity = INITIAL_CAPACITY;
    dictionary->size = 0;
    dictionary->buckets = calloc(dictionary->capacity, sizeof(struct DictionaryNode *));
    return dictionary;
}

// 🆕 Create new IDictionary instance
struct IDictionary *collection_dictionary_new() {
    return (struct IDictionary *) init(alloc());
}

// 🧹 Free dictionary instance (note: DictionaryNode entries already freed by clear)
void collection_dictionary_free(struct IDictionary **dictionary) {
    if (dictionary == NULL || *dictionary == NULL) return;

    struct Dictionary *this = (struct Dictionary *) *dictionary;
    mutex_destroy(&this->mutex);
    free(*dictionary);

    *dictionary = NULL;
}
