#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "collection/dictionary.h"
#include "collection/mutex.h"

#define INITIAL_CAPACITY 16

/**
 * @struct DictionaryNode
 * @brief Node representing a key-value pair in a hash table.
 *
 * Each node stores a heap-allocated key string, a generic pointer value,
 * and a pointer to the next node for handling hash collisions via chaining.
 */
struct DictionaryNode {
    const char *key;                    /**< 🔑 Heap-allocated key string */
    const void *value;                  /**< 📦 Pointer to the associated value (generic) */
    struct DictionaryNode *next;        /**< ➡️ Pointer to the next node in the collision chain */
};

/**
 * @struct Dictionary
 * @brief Hash table dictionary with separate chaining for collision resolution.
 *
 * Contains a base IDictionary interface, an array of bucket pointers
 * each pointing to a linked list of DictionaryNodes,
 * as well as capacity and size metadata.
 */
struct Dictionary {
    struct IDictionary base;            /**< 🧩 Base interface for dictionary operations */
    struct DictionaryNode **buckets;    /**< 🗃️ Array of bucket heads (linked lists) */
    size_t capacity;                    /**< 📏 Number of buckets in the hash table */
    size_t size;                        /**< 📊 Number of key-value pairs stored */
};

// 🔒 Mutex protecting DictionaryNode structure for thread safety
static Mutex mutex;

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
    mutex_lock_shared(&mutex);
    const struct Dictionary *instance = (struct Dictionary *) self;
    const unsigned long index = hash(key) % instance->capacity;

    const void *value = NULL;
    for (const struct DictionaryNode *cursor = instance->buckets[index]; cursor; cursor = cursor->next) {
        if (strcmp(cursor->key, key) == 0) {
            value = cursor->value; // 🎯 Found value
            break;
        }
    }

    mutex_unlock(&mutex);
    return value;
}

// ➕ Insert key-value pair with write-lock
static bool put(struct IDictionary *self, const char *key, const void *value) {
    const struct Dictionary *instance = (struct Dictionary *) self;
    const unsigned long index = hash(key) % instance->capacity;

    struct DictionaryNode *DictionaryNode = malloc(sizeof(struct DictionaryNode));
    if (DictionaryNode == NULL) {
        fprintf(stderr, "DictionaryNode allocation failed.\n");
        return false;
    }

    DictionaryNode->key = strdup(key); // 🆕 Duplicate key string for ownership
    DictionaryNode->value = value;
    DictionaryNode->next = NULL;

    mutex_lock(&mutex);

    // 🔗 Append new DictionaryNode to bucket's linked list
    struct DictionaryNode **cursor;
    for(cursor = &instance->buckets[index]; *cursor; cursor = &(*cursor)->next) {}
    *cursor = DictionaryNode;

    mutex_unlock(&mutex);

    return true;
}

// 🔎 Check existence of key with read-lock
static bool containsKey(const struct IDictionary *self, const char *key) {
    mutex_lock_shared(&mutex);

    const struct Dictionary *instance = (const struct Dictionary *)self;
    const unsigned long index = hash(key) % instance->capacity;

    bool found = false;
    for (const struct DictionaryNode *cursor = instance->buckets[index]; cursor; cursor = cursor->next) {
        if (strcmp(cursor->key, key) == 0) {
            found = true; // ✅ Key found
            break;
        }
    }

    mutex_unlock(&mutex);
    return found;
}

// Remove key and return associated value with write-lock
void *removeItem(struct IDictionary *self, const char *key) {
    mutex_lock(&mutex);

    const struct Dictionary *instance = (struct Dictionary *) self;
    const unsigned long index = hash(key) % instance->capacity;
    void *value = NULL;

    for (struct DictionaryNode **cursor = &instance->buckets[index]; *cursor; cursor = &(*cursor)->next) {
        struct DictionaryNode *DictionaryNode = *cursor;
        if (strcmp(DictionaryNode->key, key) == 0) {
            *cursor = DictionaryNode->next;       // 🔗 Remove DictionaryNode from list
            value = (void *)DictionaryNode->value;

            free((void *) DictionaryNode->key);   // 🧹 Free duplicated key
            free(DictionaryNode);                 // 🧹 Free DictionaryNode
            break;
        }
    }

    mutex_unlock(&mutex);
    return value; // 🔍 Not found
}

// 🔄 Replace value of existing key with write-lock
static void *replace(const struct IDictionary *self, const char *key, const void *value) {
    mutex_lock(&mutex); // 🔒 Acquire write-lock for safe mutation
    const struct Dictionary *instance = (struct Dictionary *)self;
    const unsigned long index = hash(key) % instance->capacity;
    void *oldValue = NULL;

    // 🔍 Traverse the bucket's linked list
    for (struct DictionaryNode *cursor = instance->buckets[index]; cursor; cursor = cursor->next) {
        if (strcmp(cursor->key, key) == 0) {
            oldValue = (void *)cursor->value;
            cursor->value = value; // 🔄 Update value

            break;
        }
    }

    mutex_unlock(&mutex);
    return oldValue; // 🔍 Not found
}

// 🧹 Clear all entries with write-lock, optional callback on each value
static void clear(const struct IDictionary *self, void (*callback)(void *value)) {
    mutex_lock(&mutex);
    struct Dictionary *instance = (struct Dictionary *)self;
    for (size_t i = 0; i < instance->capacity; ++i) {
        struct DictionaryNode *current = instance->buckets[i];
        while (current != NULL) {
            struct DictionaryNode *DictionaryNode = current;
            current = current->next;
            free((void *) DictionaryNode->key); // 🧹 Free key
            if (callback) callback((void *) DictionaryNode->value); // 🔔 User cleanup
            free(DictionaryNode); // 🧹 Free DictionaryNode
        }
        instance->buckets[i] = NULL; // 🔄 Reset bucket pointer
    }
    free(instance->buckets); // 🧹 Free DictionaryNode array
    instance->buckets = NULL;
    mutex_unlock(&mutex);
}

// 🔧 Initialize dictionary base functions
struct Dictionary *collection_dictionary_init(struct Dictionary *dictionary) {
    if (dictionary == NULL) return NULL;
    mutex_init(&mutex);
    dictionary->base.get = get;
    dictionary->base.put = put;
    dictionary->base.containsKey = containsKey;
    dictionary->base.removeItem = removeItem;
    dictionary->base.replace = replace;
    dictionary->base.clear = clear;
    return dictionary;
}

// 🆕 Allocate and initialize new dictionary
struct Dictionary *collection_dictionary_alloc() {
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
    return (struct IDictionary *)collection_dictionary_init(collection_dictionary_alloc());
}

// 🧹 Free dictionary instance (note: DictionaryNode entries already freed by clear)
void collection_dictionary_free(struct IDictionary **dictionary) {
    mutex_destroy(&mutex);
    free(*dictionary);
    *dictionary = NULL;
}
