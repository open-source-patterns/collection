/**
 * @file IDictionary.h
 * @brief Interface definition for a generic key-value dictionary.
 *
 * This header declares the IDictionary struct, which provides
 * an abstract interface for managing string-keyed collections
 * of arbitrary pointer values. It supports typical dictionary
 * operations such as lookup, insertion, removal, and clearing.
 *
 * Implementations of this interface can vary in storage and
 * performance characteristics but share this common API.
 */

#pragma once

#include <stdbool.h>

/**
 * @brief Interface for a generic key-value dictionary.
 *
 * Stores associations between string keys and arbitrary pointer values.
 */
struct IDictionary {
    /**
     * @brief Retrieves the value associated with the given key.
     *
     * @param self Pointer to the dictionary instance.
     * @param key Null-terminated string key to look up.
     * @return Pointer to the value if key exists; otherwise NULL.
     */
    const void *(*get)(const struct IDictionary *self, const char *key);

    /**
     * @brief Adds or updates a key-value pair in the dictionary.
     *
     * @param self Pointer to the dictionary instance.
     * @param key Null-terminated string key to add or update.
     * @param value Pointer to the value to associate with the key.
     * @return true if insertion or update was successful; false on failure (e.g. memory allocation failure).
     */
    bool (*put)(struct IDictionary *self, const char *key, const void *value);

    /**
     * @brief Checks whether the dictionary contains a given key.
     *
     * @param self Pointer to the dictionary instance.
     * @param key Null-terminated string key to check.
     * @return true if the key exists; false otherwise.
     */
    bool (*containsKey)(const struct IDictionary *self, const char *key);

    /**
     * @brief Removes the key-value pair associated with the given key.
     *
     * @param self Pointer to the dictionary instance.
     * @param key Null-terminated string key to remove.
     * @return Pointer to the removed value if the key was found; otherwise NULL.
     *         Caller is responsible for managing the memory of the returned value.
     */
    void *(*removeItem)(struct IDictionary *self, const char *key);

    /**
     * @brief Replaces the value associated with a key, returning the previous value.
     *
     * @param self Pointer to the dictionary instance.
     * @param key Null-terminated string key to update.
     * @param value New value pointer to associate with the key.
     * @return Pointer to the old value if key was found; otherwise NULL.
     *         Caller is responsible for managing the memory of the returned value.
     */
    void *(*replace)(const struct IDictionary *self, const char *key, const void *value);

    /**
     * @brief Clears all key-value pairs from the dictionary.
     *
     * @param self Pointer to the dictionary instance.
     * @param callback Optional function pointer called for each value before removal.
     *                 May be NULL if no cleanup is needed.
     */
    void (*clear)(const struct IDictionary *self, void (*callback)(void *value));
};

/**
 * @brief Creates a new instance of an IDictionary implementation.
 *
 * Allocates and initializes a new dictionary object.
 *
 * @return Pointer to the newly created IDictionary instance.
 *         Returns NULL if allocation fails.
 */
struct IDictionary *collection_dictionary_new();

/**
 * @brief Frees an IDictionary instance and releases its resources.
 *
 * After calling this function, the dictionary pointer is set to NULL.
 *
 * @param dictionary Double pointer to the IDictionary instance to free.
 *                   Must not be NULL.
 */
void collection_dictionary_free(struct IDictionary **dictionary);
