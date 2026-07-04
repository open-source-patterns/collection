/**
* @file dictionary.h
* @internal
* @brief Dictionary Header
*
* @author Saad Shams https://linkedin.com/in/muizz
* @copyright BSD 3-Clause License
*/
#pragma once

#include "collection/i_dictionary.h"
#include "collection/i_platform.h"

/**
 * @struct DictionaryNode
 * @brief Node in a hash table bucket chain.
 *
 * Stores a key-value pair and a pointer to the next node used for
 * collision resolution via separate chaining.
 */
struct DictionaryNode {
    const char *key;                    /**< Heap-allocated key string. */
    const void *value;                  /**< Associated value. */
    struct DictionaryNode *next;        /**< Next node in the bucket chain. */
};

/**
 * @struct Dictionary
 * @brief Hash table implementation of IDictionary.
 *
 * Stores key-value pairs in an array of buckets, with collisions resolved
 * using separate chaining. Access is synchronized with a mutex.
 */
struct Dictionary {
    struct IDictionary super;           /**< IDictionary interface implemented by this type. */
    struct DictionaryNode **buckets;    /**< Array of bucket heads. */
    size_t capacity;                    /**< Number of buckets. */
    size_t size;                        /**< Number of stored key-value pairs. */
    Mutex mutex;                        /**< Mutex protecting dictionary operations. */
};
