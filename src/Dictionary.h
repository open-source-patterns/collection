/**
 * @file dictionary.h
 * @brief Factory and destructor functions for IDictionary implementations.
 */

#pragma once

#include "collection/Mutex.h"
#include "collection/IDictionary.h"

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
    Mutex mutex;                        /**< 🔒 Mutex for thread safety */
};
